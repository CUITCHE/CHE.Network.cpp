#include "TimeVerification.h"
#include "htime.h"
CHE_NAMESPACE_BEGIN

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

CHE_NAMESPACE_END
