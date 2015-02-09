#ifndef HLog_H__
#define HLog_H__

/********************************************************************
	created:	2014/11/27
	created:	27:11:2014   20:55
	file base:	HLog
	author:		CHE
	
	purpose:	��־��
*********************************************************************/
#include "hglobal.h"
#include "htime.h"
#include <mutex>
#include <memory>
#include <thread>
#include <fstream>
#include <sstream>
using std::mutex;
using std::thread;
using std::string;
using std::fstream;
using std::unique_ptr;
using std::stringstream;
NAMESPACE_BEGIN_CHE

constexpr uint32 LOG_BUFFER_LENGTH = 1024 * 8;
class HLog final
{
	CHE_DISABLE_COPY(HLog);
	CHE_DISABLE_COPY_RREF(HLog);
public:
	typedef std::function<void(const string &msg)> logOutputCallback;
	struct Stream {
		enum BufferFlush :uint32 {
			//���������flushed
			OutputBufferFlushed = 1,

			//���뻺����flushed
			InputBufferFlushed = 1 << 1,

			//׼���˳���־����߳�
			LogExiting = 1 << 2,

			//���������δflush
			OutputBufferFlush = ~OutputBufferFlushed,

			//���뻺����δflush
			InputBufferFlush = ~InputBufferFlushed,

			//maxnumberic of uint32
			Check_numberic = (uint32)-1
		};
		Stream()
			:shut_down(0)
			,h_in(new stringstream)
			,h_out(new stringstream)
			,log_input_mutex(new mutex)
			,log_output_mutex(new mutex){}
		~Stream() {

			che_pointer_del(threadData, callback, h_in, h_out, log_input_mutex, log_output_mutex);
		}
		//��������������֤�ڹ����߳��е��ã������߳̿��ܻ������
		void swap() { 
			std::lock_guard<mutex> locker(*log_input_mutex);
			std::swap(h_in, h_out); 
		}

		void close(){
			shut_down |= LogExiting;
			threadData->join();
		}
		uint32 shut_down;		//������λ��Ϊ1ʱ��ʾtrue
		stringstream *h_in;		//д����־
		stringstream *h_out;	//�����־��Ӳ��
		logOutputCallback *callback = nullptr;
		mutex *log_input_mutex;
		mutex *log_output_mutex;
		thread *threadData;
	};
public:
	enum class LogFileRedirect :uint8 {
		Stderr,
		Stdout
	};
	//�����ļ�����������ļ�������HLog������HLog*
	//���������ļ����ѱ��򿪣����᷵��nullptr
	static HLog* logExecuteReg(const char *filename);

	//�����ݵ��������callback
	static bool logRedirect(logOutputCallback &callback, HLog *log);

	//������ض���stderr����stdout
	//�ض���ʧ�ܷ���false���п�ָ����
	//ע�⣬����һ���Բ������Ḳ��ԭ�е����ָ��
	static bool logRedirect(LogFileRedirect rdtype, HLog *log);

	//�ر�һ�� ��֪��HLog����ָ��
	static void logClose(HLog *log);

	//д����־
	//������Ϣ ��־
	template<typename... Args>
	void warningLog(Args... args) { this->fmt_log(WarningMsg, args...); }

	//������Ϣ ��־
	template<typename... Args>
	void fatalLog(Args... args) { this->fmt_log(FatalMsg, args...); }

	//Σ������Ϣ ��־
	template<typename... Args>
	void criticalLog(Args... args) { this->fmt_log(CriticalMsg, args...); }

	//������Ϣ ��־����Releaseģʽ������Ϣ���ᱻ���ԣ�
	//Releaseģʽ�£��붨��NODEBUG����ȡ������־���
#ifndef NODEBUG
	template<typename... Args>
	void debugLog(Args... args) { this->fmt_log(DebugMsg, args...); }
#else
	template<typename... Args>
	void debugLog(Args... args) {}
#endif // !NODEBUG

	//һ������Ϣ ��־
	template<typename... Args>
	void generalLog(Args... args) { this->fmt_log(GeneralMsg, args...); }

	~HLog();
protected:
	//��ʽ���������־
	template<typename... Args>
	void fmt_log(MsgType type, Args... args) {
		std::lock_guard<mutex> locker(*d_ptr->log_input_mutex);

		auto &ss = *d_ptr->h_in;
		ss << HTime::fmt_currentDateTime() << std::endl;
		switch (type)
		{
		case DebugMsg:
			ss << "DebugMsg:";
			break;
		case WarningMsg:
			ss << "WarningMsg:";
			break;
		case FatalMsg:
			ss << "FatalMsg:";
			break;
		case CriticalMsg:
			ss << "CriticalMsg:";
			break;
		default:
			ss << "GeneralMsg:";
			break;
		}
		log_analytical(args...);
		ss << std::endl << std::endl;
	}

	//�����ļ�ָ�룬Ĭ�������stderr��
	//����ָ���ļ�ָ�룬�������Ӧ���ļ���
	HLog(fstream *fp);

	//ǿ����־�߳�flush���������ݵ��ļ�
	//һ���ڻ�������������־�̵߳�����ϲ�����־�������ٶ�
	//���ṩ���û����ɳ����Լ�����
	void flush();

	//ͳһ������ļ��ӿ�
	inline void mayberd(const string &buffer);

	//����stringstream֧��operator<<�����ݸ�ʽ�������Stream��ȥ
	template<typename T, typename... Args>
	void log_analytical(const T &value, Args... args) {
		(*d_ptr->h_in) << value;
		log_analytical(args...);
	}
	void log_analytical(void) {}

	//�̺߳�������ʱ�������ļ������־
	void output();

	//�ض�����
	void log_stdout(const string &msg) {
		std::cout << msg;
	}
	void log_stderr(const string &msg) {
		std::cerr << msg;
	}
private:
	unique_ptr<Stream> d_ptr;
	fstream *fp;
};
NAMESPACE_END_CHE
#endif // HLog_H__
