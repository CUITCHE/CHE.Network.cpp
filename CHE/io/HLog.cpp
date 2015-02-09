#include "HLog.h"
#include <stdlib.h>
NAMESPACE_BEGIN_CHE

HLog *logger = nullptr;

HLog::HLog(fstream * fp)
:d_ptr(new Stream())
,fp(fp)
{}

HLog* HLog::logExecuteReg(const char *filename)
{
	
	fstream *fp = new fstream(filename, std::ios_base::out);
	if (!fp->is_open()) {
		return nullptr;
	}
	HLog *logg = new HLog(fp);
	logg->d_ptr->threadData = new thread(move_quick(std::bind(&HLog::output, logg)));
	return logg;
}

bool HLog::logRedirect(logOutputCallback & callback, HLog *log)
{
	if (log == nullptr) {
		return false;
	}
	auto &calback = log->d_ptr->callback;
	calback == nullptr ? calback = new logOutputCallback() : che_noop();
	std::lock_guard<mutex> locker(*log->d_ptr->log_output_mutex);
	*calback = callback;
	return true;
}

bool HLog::logRedirect(LogFileRedirect rdtype, HLog *log)
{
	if (!log || log->fp == nullptr) {
		return false;
	}
	if (log->d_ptr->callback == nullptr) {
		log->d_ptr->callback = new logOutputCallback();
	}
	switch (rdtype)
	{
	case LogFileRedirect::Stderr:
		*log->d_ptr->callback = move_quick(std::bind(&HLog::log_stderr, log, std::placeholders::_1));
		break;
	case LogFileRedirect::Stdout:
		*log->d_ptr->callback = move_quick(std::bind(&HLog::log_stdout, log, std::placeholders::_1));
		break;
	default:
		return false;
	}
	return true;
}

void HLog::logClose(HLog * log)
{
	che_pointer_del(log);
}

HLog::~HLog()
{
	d_ptr->close();
	fp->close();
	delete fp;
}

void HLog::flush()
{
	if (d_ptr->h_out->tellp() <= 0) {
		return;
	}

	std::lock_guard<mutex> locker(*d_ptr->log_output_mutex);
	
	string buffer = d_ptr->h_out->str();
	mayberd(buffer);
	//清空
	d_ptr->h_out->str("");
}

void HLog::mayberd(const string &buffer)
{
	if (d_ptr->callback != nullptr) {
		(*d_ptr->callback)(buffer);
		return;
	}

	*fp << buffer;
}

void HLog::output()
{
	stringstream &log_out_buffer = *d_ptr->h_out;

	uint32 &shut_down_signal = d_ptr->shut_down;
	int sleep_time = 0;
	forever{
		if ((shut_down_signal & Stream::LogExiting) == Stream::LogExiting) {
			flush();
			if (d_ptr->h_in->tellp() > 0) {
				//输入缓冲区还有未完成输出的数据
				//由于是退出工作线程的事儿，所以以后的数据就不用管了，
				d_ptr->swap();
				flush();
			}
			break;
		}
		if (d_ptr->h_in->tellp() >= LOG_BUFFER_LENGTH && d_ptr->h_out->tellp() <= 0) {
			d_ptr->swap();
		}
		if (fp->tellp() > LOG_BUFFER_LENGTH << 2) {
			//强制输出log到文件
			fp->flush();
		}
		if (d_ptr->h_out->tellp() <= 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			sleep_time += 5;
			//在空闲的时候才进行磁盘io操作
			if (sleep_time > 2 * 100) {
				//将数据写到磁盘上
				if (fp->tellp() > 0) {
					fp->flush();
				}
				else if(d_ptr->h_in->tellp() > 0) {
					//这里是针对输出数据量小，并且磁盘空闲
					d_ptr->swap();
				}
				sleep_time = 0;
			}
			continue;
		}	
		sleep_time = 0;
		flush();
	}
}

NAMESPACE_END_CHE
