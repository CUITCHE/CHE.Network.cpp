#include "HCoreApplication.h"
#include "htime.h"
#include <cassert>
#include <string>
using std::string;
NAMESPACE_BEGIN_CHE
#pragma region INITL
HCoreApplication* HCoreApplication::self = nullptr;
const std::thread::id HCoreApplication::theMainThread = std::this_thread::get_id();
time_t HCoreApplication::startRunTime = 0;
#pragma endregion INITL

HCoreApplication::HCoreApplication()
	:HObject(nullptr)
{
	if (self != nullptr){
		Warning("instance of HCoreApplication must be unique!");
		quit(-1);
		return;
	}
	if (theMainThread != std::this_thread::get_id()){
		Warning("The instance of HCoreApplication must be created where main thread is!");
		return;
	}
	self = this;
}

HCoreApplication::~HCoreApplication()
{
	if (self == this){
		self = nullptr;
	}
}

void HCoreApplication::quit(uint32 exitCode /*= 0*/)
{
	if (hApp == nullptr){
		return;
	}
	F_DF(hApp);
	df->exit_t = exitCode;
	df->shut_down = true;
}

const uint32 HCoreApplication::lifeTime()
{
	return (uint32)(HTime::currentTime() - startRunTime);
}

int HCoreApplication::exec()
{
	startRunTime = HTime::currentTime();
	this->d_func()->thread_run_access = true;
	return HObject::exec();
}

NAMESPACE_END_CHE