#include "TimeVerification.h"
#include "htime.h"
NAMESPACE_BEGIN_CHE

void Timeverification::addServerURL(const char *url)
{
	auto &server_name = HTime::server_name;
	auto iter = std::find(server_name.begin(), server_name.end(), url);
	if (iter == server_name.end()){
		server_name.push_back(url);
	}
}

HDateTime Timeverification::getTime(int UTC /*= 8*/)
{
	return HTime::getNetUTCTime(UTC);
}

NAMESPACE_END_CHE