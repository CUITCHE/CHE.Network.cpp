#include "HByteConvert.h"
#include "registerpacket.h"
using namespace std;
using namespace che;
template<typename T>
void write_t(T d) {
	cout << d;
}
template<>
void write_t<double>(double d) {
	printf("%.2lf----\n", d);
}
int main()
{
	DATATYPESET *set = new DATATYPESET;
	*set = 9;
	char str[4];

	int *dst = (int *)str;
	int *src = (int *)set;
	*dst = *src;

	*set = 0;
	*src = *dst;

	RegisterPacket *packet = new RegisterPacket;
	packet->setLevels(589);
	packet->setName("ºÎ¿¡Çï");
	packet->setPassword("123456789124dfefe");
	packet->set___int64(489);
	packet->set_byte(12);
	packet->set_double(12.25);
	packet->set_short(25);
	packet->set_float(25.02f);
	auto &data = packet->Write();
	RegisterPacket *packet2 = new RegisterPacket;
	packet2->Read(data);
	return 0;
}