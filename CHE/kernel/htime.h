#ifndef htime_H__
#define htime_H__
/********************************************************************
	created:	2014/10/08
	created:	8:10:2014   18:45
	file ext:	h
	author:		CHE
	
	purpose:	����ƽ̨��ֻ�ṩ�򵥵�ʱ��time_t��ֵ���أ���fmtʱ��
				Windowsƽ̨���ṩ��Ч�ĸ߾��ȡ������뼶���ʱ���ʱ
				���ṩʱ����㣬����Ҫʱ����㣬����class Time
*********************************************************************/
#include "hglobal.h"
#include "HDateTime.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
using std::vector;
using std::string;
using std::shared_ptr;
using namespace std::chrono;

union _LARGE_INTEGER;

CHE_NAMESPACE_BEGIN

class HTime
{
	friend class Timeverification;
public:
	//���ڱ�ʾҪ����ľ��ȣ�����Ĭ��Ϊ���뼶
	enum OutputPrecision
	{
		//��
		Normalsecond,
		//����
		Millisecond,
		//΢��
		Microsecond,
		//����
		Nanosecond
	};
#ifdef WIN32
	HTime(OutputPrecision precision = Millisecond);
	~HTime();
	//��ʼ��ʱ
	void start();
	//���¿�ʼ��ʱ
	void restart();
	//�����start����restart�����ú��ʱ����
	long double elapsed();
protected:
	//���þ�����ز���
	inline void setPrecision(OutputPrecision precision);
private:
	_LARGE_INTEGER *tStart, *tEnd;
	OutputPrecision outputPrecision;
	double precisionNum;	//Ϊ�˱�ʾ���ȣ�Ҫ���Ե���������Ϊ������λ
	long long tFreq;	//��ʾCPUƵ��
#else
public:
	HTime(OutputPrecision precision = Millisecond){}
	~HTime(){}
	//��ʼ��ʱ
	void start(){}
	//���¿�ʼ��ʱ
	void restart(){}
	//�����start����restart�����ú��ʱ����
	long double elapsed(){}
#endif // WIN32

public:
	//��õ�ǰϵͳʱ����time_t���أ���ȷ������
	static time_t currentTime();

	//������� 2014/02/12 �����string���滻���������е�"/"
	static string currentDate(char type = '/');

	//���ʱ�� 12:02:13������true����12Сʱ�ƣ�false����24Сʱ��
	static string& currentTime(bool _in);

	//������ں�ʱ�䣬����Ϊ�̶���λ��24Сʱ��
	static string currentDateTime(char type = '_');

	//���ء�2014/11/30 14:05:30 �����ա���string
	static string fmt_currentDateTime();

protected:
	//�������UTCʱ�䣬����utc��ʱ����
	//����һ����������ʱ���룬ÿ��ֵ1���ֽڴ洢��uint64�ĵ�48λ��
	//����year, month,day�Ⱥ�����ȡ�������ֵ��Ҳ���Ժϳ�����������ʱ��
	static HDateTime getNetUTCTime(int UTC = 8);
private:
	static vector<string> server_name;
};
CHE_NAMESPACE_END
#endif // htime_H__
