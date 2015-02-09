#ifndef HLog_H__
#define HLog_H__

/********************************************************************
	created:	2014/11/27
	created:	27:11:2014   20:55
	file base:	HLog
	author:		CHE
	
	purpose:	日志类
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
			//输出缓冲区flushed
			OutputBufferFlushed = 1,

			//输入缓冲区flushed
			InputBufferFlushed = 1 << 1,

			//准备退出日志输出线程
			LogExiting = 1 << 2,

			//输出缓冲区未flush
			OutputBufferFlush = ~OutputBufferFlushed,

			//输入缓冲区未flush
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
		//交换缓冲区，保证在工作线程中调用，其它线程可能会崩溃！
		void swap() { 
			std::lock_guard<mutex> locker(*log_input_mutex);
			std::swap(h_in, h_out); 
		}

		void close(){
			shut_down |= LogExiting;
			threadData->join();
		}
		uint32 shut_down;		//二进制位上为1时表示true
		stringstream *h_in;		//写入日志
		stringstream *h_out;	//输出日志到硬盘
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
	//传入文件名，打开这个文件，激活HLog并返回HLog*
	//如果传入的文件名已被打开，将会返回nullptr
	static HLog* logExecuteReg(const char *filename);

	//将数据的输出交给callback
	static bool logRedirect(logOutputCallback &callback, HLog *log);

	//将输出重定向到stderr或者stdout
	//重定向失败返回false，有空指针检查
	//注意，这是一次性操作，会覆盖原有的输出指针
	static bool logRedirect(LogFileRedirect rdtype, HLog *log);

	//关闭一个 已知的HLog对象指针
	static void logClose(HLog *log);

	//写入日志
	//警告信息 日志
	template<typename... Args>
	void warningLog(Args... args) { this->fmt_log(WarningMsg, args...); }

	//致命信息 日志
	template<typename... Args>
	void fatalLog(Args... args) { this->fmt_log(FatalMsg, args...); }

	//危险性信息 日志
	template<typename... Args>
	void criticalLog(Args... args) { this->fmt_log(CriticalMsg, args...); }

	//调试信息 日志，在Release模式调试信息将会被忽略，
	//Release模式下，请定义NODEBUG宏来取消此日志输出
#ifndef NODEBUG
	template<typename... Args>
	void debugLog(Args... args) { this->fmt_log(DebugMsg, args...); }
#else
	template<typename... Args>
	void debugLog(Args... args) {}
#endif // !NODEBUG

	//一般性信息 日志
	template<typename... Args>
	void generalLog(Args... args) { this->fmt_log(GeneralMsg, args...); }

	~HLog();
protected:
	//格式化输出到日志
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

	//传入文件指针，默认输出到stderr，
	//传入指针文件指针，输出到相应的文件中
	HLog(fstream *fp);

	//强制日志线程flush缓冲区数据到文件
	//一般在缓冲区已满，日志线程的输出赶不上日志的输入速度
	//不提供给用户，由程序自己控制
	void flush();

	//统一输出到文件接口
	inline void mayberd(const string &buffer);

	//将被stringstream支持operator<<的数据格式，输出到Stream中去
	template<typename T, typename... Args>
	void log_analytical(const T &value, Args... args) {
		(*d_ptr->h_in) << value;
		log_analytical(args...);
	}
	void log_analytical(void) {}

	//线程函数，定时定量向文件输出日志
	void output();

	//重定向函数
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
