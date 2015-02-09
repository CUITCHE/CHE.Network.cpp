#include "HByteConvert.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <cassert>

NAMESPACE_BEGIN_CHE
DATATYPESET HByteConvert::_convertDataTypeCollection;
string HByteConvert::ws2s(const wstring &ws)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	assert(_Dest != NULL);
	CheZeroMemory(_Dest, _Dsize);

	uint32 returnValue(0);
	wcstombs_s(&returnValue, _Dest, _Dsize, _Source, _TRUNCATE);

	string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return (result);
}

wstring HByteConvert::s2ws(const string &s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	assert(_Dest != NULL);
	CheZeroMemory(_Dest, _Dsize);

	uint32 returnValue(0);
	mbstowcs_s(&returnValue, _Dest, _Dsize, _Source, _TRUNCATE);
	std::wstring result = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, "C");
	return (result);
}

/*
const byte* HByteConvert::shortToByte(const short data)
{
//
// 	byte *pBuf = new byte[2];
// 	assert(pBuf != NULL);
// 
// 	pBuf[0] = (byte)((data & 0xff00) >> 8);
// 	pBuf[1] = (byte)(data & 0x00ff);

	_convertDataTypeCollection = data;
	return _convertDataTypeCollection;
}

void HByteConvert::shortToByte(const short data, byte *pBuf)
{
	assert(pBuf != NULL);
//
// 	pBuf[0] = (byte)((data & 0xff00) >> 8);
// 	pBuf[1] = (byte)(data & 0x00ff);

	_convertDataTypeCollection = data;
	memcpy2(pBuf, _convertDataTypeCollection);
}

short HByteConvert::byteToShort(const byte *pBuf)
{
	assert(pBuf != NULL);

//
// 	short high = (short)((pBuf[0] << 8) & 0xff00);
// 	short low = (short)(pBuf[1] & 0xff);
// 	short t = high | low;
	_convertDataTypeCollection.memcpy2(pBuf);
	return _convertDataTypeCollection;
}

byte* HByteConvert::intToByte(const int data)
{
//
// 	byte *pBuf = new byte[4];
// 	assert(pBuf != NULL);
// 
// 	pBuf[3] = (byte)((data & 0xff));
// 	pBuf[2] = (byte)((data & 0xff00) >> 8);
// 	pBuf[1] = (byte)((data & 0xff0000) >> 16);
// 	pBuf[0] = (byte)((data & 0xff000000) >> 24);
	_convertDataTypeCollection = data;
	return _convertDataTypeCollection;
}

void HByteConvert::intToByte(const int data, byte *pBuf)
{
	assert(pBuf != NULL);

// 	pBuf[3] = (byte)((data & 0xff));
// 	pBuf[2] = (byte)((data & 0xff00) >> 8);
// 	pBuf[1] = (byte)((data & 0xff0000) >> 16);
// 	pBuf[0] = (byte)((data & 0xff000000) >> 24);
	_convertDataTypeCollection = data;
	memcpy4(pBuf, _convertDataTypeCollection);
}


int HByteConvert::byteToInt(const byte *pBuf)
{
	assert(pBuf != NULL);
//
// 	int h24 = pBuf[0] << 24 & 0xff000000;
// 	int h16 = pBuf[1] << 16 & 0xff0000;
// 	int l8 = pBuf[2] << 8 & 0xff00;
// 	int l = pBuf[3] & 0xff;
// 
// 	int t = h24 | h16 | l8 | l;
	_convertDataTypeCollection.memcpy4(pBuf);

	return _convertDataTypeCollection;
}

byte* HByteConvert::long64ToByte(const long long &data)
{
//	byte *pBuf = new byte[8];
//	assert(pBuf != NULL);
//
// 	pBuf[7] = (byte)(data & 0xffL);
// 	pBuf[6] = (byte)((data & 0xff00L) >> 8);
// 	pBuf[5] = (byte)((data & 0xff0000L) >> 16);
// 	pBuf[4] = (byte)((data & 0xff000000L) >> 24);
// 	pBuf[3] = (byte)((data & 0xff00000000L) >> 32);
// 	pBuf[2] = (byte)((data & 0xff0000000000L) >> 40);
// 	pBuf[1] = (byte)((data & 0xff000000000000L) >> 48);
// 	pBuf[0] = (byte)((data & 0xff00000000000000L) >> 56);
	_convertDataTypeCollection = data;
	return _convertDataTypeCollection;
}

void HByteConvert::long64ToByte(const long long &data, byte *pBuf)
{
	assert(pBuf != NULL);

// 	pBuf[7] = (byte)(data & 0xffL);
// 	pBuf[6] = (byte)((data & 0xff00L) >> 8);
// 	pBuf[5] = (byte)((data & 0xff0000L) >> 16);
// 	pBuf[4] = (byte)((data & 0xff000000L) >> 24);
// 	pBuf[3] = (byte)((data & 0xff00000000L) >> 32);
// 	pBuf[2] = (byte)((data & 0xff0000000000L) >> 40);
// 	pBuf[1] = (byte)((data & 0xff000000000000L) >> 48);
// 	pBuf[0] = (byte)((data & 0xff00000000000000L) >> 56);
	_convertDataTypeCollection = data;
	memcpy8(pBuf, _convertDataTypeCollection);
}

long long HByteConvert::byteToLong64(const byte *pBuf)
{
	assert(pBuf != NULL);

// 	long long h56 = (long long)pBuf[0] << 56 & 0xff00000000000000L;
// 	long long h48 = (long long)pBuf[1] << 48 & 0xff000000000000L;
// 	long long h40 = (long long)pBuf[2] << 40 & 0xff0000000000L;
// 	long long h32 = (long long)pBuf[3] << 32 & 0xff00000000L;
// 	long long l24 = (long long)pBuf[4] << 24 & 0xff000000L;
// 	long long l16 = (long long)pBuf[5] << 16 & 0xff0000L;
// 	long long l08 = (long long)pBuf[6] <<  8 & 0xff00L;
// 	long long l00 = (long long)pBuf[7] & 0xffL;
// 
// 	long long t = h32 | h40 | h48 | h56 | l00 | l08 | l16 | l24;
	_convertDataTypeCollection.memcpy8(pBuf);
	return _convertDataTypeCollection;
}
*/
/*
byte* HByteConvert::stringToByte(const string &data)
{
	QString str = QString::fromStdString(data);
	QByteArray array = str.toLocal8Bit();
	char *pchar = new char[array.size() + 1];
	pchar[array.size()] = 0;
	memcpy_s(pchar, array.size(), array.constData(), array.size());
	return (byte *)pchar;
// 	wstring wstr = s2ws(data);
// 	wchar_t *wchar = const_cast<wchar_t *>(wstr.c_str());
// 	assert(wchar != NULL);
// 	int size = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
// 	char *pchar = new char[size + 1];
// 	if (!WideCharToMultiByte(CP_ACP, 0, wchar, -1, pchar, size, NULL, NULL))
// 	{
// 		delete[]pchar;
// 	}

//	return (byte *)pchar;
}

void HByteConvert::stringToByte(const string &data, byte *pBuf)
{
	assert(pBuf == NULL);
	QString str = QString::fromStdString(data);
	QByteArray array = str.toLocal8Bit();
	pBuf = new byte[array.size() + 1];
	memcpy_s(pBuf, array.size(), array.constData(), array.size());
// 	assert(pBuf == NULL);
// 	wstring wstr = s2ws(data);
// 	wchar_t *wchar = const_cast<wchar_t *>(wstr.c_str());
// 	assert(wchar != NULL);
// 	int size = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);
// 	char *pchar = new char[size + 1];
// 	if (!WideCharToMultiByte(CP_ACP, 0, wchar, -1, pchar, size, NULL, NULL))
// 	{
// 		delete[]pchar;
// 	}
// 
// 	pBuf = (byte *)pchar;
}

void HByteConvert::byteToString(string &data, const byte *pBuf)
{
	data = utf8((const char *)pBuf).toStdString();
// 	char *pchar = (char *)pBuf;
// 	int size = MultiByteToWideChar(CP_ACP, 0, pchar, -1, NULL, 0);
// 	wchar_t *wchar = new wchar_t[size + 1];
// 	assert(wchar != NULL);
// 	if (!MultiByteToWideChar(CP_ACP, 0, pchar, -1, wchar, size))
// 	{
// 		delete[]wchar;
// 	}
// 	wstring wstr = wchar;
// 	data = ws2s(wstr);
// 	wchar != NULL ? delete[]wchar, wchar = NULL : 0;
}

byte* HByteConvert::stringToByte(const QString &data)
{
	QByteArray array = data.toLocal8Bit();
	char *pchar = new char[array.size() + 1];
	pchar[array.size()] = 0;
	memcpy_s(pchar, array.size(), array.constData(), array.size());
	return (byte *)pchar;
}

void HByteConvert::stringToByte(const QString &data, byte *pBuf)
{
	QByteArray array = data.toLocal8Bit();
	char *pchar = new char[array.size() + 1];
	pchar[array.size()] = 0;
	memcpy_s(pchar, array.size(), array.constData(), array.size());
}

void HByteConvert::byteToString(QString &data, const byte *pBuf)
{
	data = utf8((const char *)pBuf);
}
*/
/*
byte* HByteConvert::doubleToByte(const double &data)
{
//	byte *pBuf = new byte[9];
//	memcpy_s(pBuf, 8, &data, 8);
	_convertDataTypeCollection = data;
	return _convertDataTypeCollection;
}

void HByteConvert::doubleToByte(const double &data, byte *&pBuf)
{
//	memcpy_s(pBuf, 8, &data, 8);
	_convertDataTypeCollection = data;
	memcpy8(pBuf, _convertDataTypeCollection);
}

double HByteConvert::byteToDouble(const byte *pBuf)
{
	_convertDataTypeCollection.memcpy8(pBuf);
	return _convertDataTypeCollection;
}

byte* HByteConvert::floatToByte(const float data)
{
// 	byte *pBuf = new byte[4];
// 	memcpy_s(pBuf, 4, &data, 4);
// 	return pBuf;
	_convertDataTypeCollection = data;
	return _convertDataTypeCollection;
}

void HByteConvert::floatToByte(const float data, byte *pBuf)
{
//	memcpy_s(pBuf, 4, &data, 4);
	_convertDataTypeCollection = data;
	memcpy4(pBuf, _convertDataTypeCollection);
}

float HByteConvert::byteToFloat(const byte *pBuf)
{
// 	float value = 0;
// 	memcpy_s(&value, 4, pBuf, 4);
// 	return value;
	_convertDataTypeCollection.memcpy4(pBuf);
	return _convertDataTypeCollection;
}
*/
/*
void HByteConvert::writeByte(HDataBuffer &buf, byte data)
{
	buf.putBytes(&data, 1);
}

byte HByteConvert::readByte(HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(1);
	return *pBuf;
}

void HByteConvert::writeBytes(HDataBuffer &buf, byte *data, INT length)
{
	byte *pBuf = intToByte(length);
	buf.putBytes(pBuf,4);		//先写入长度
	
	buf.putBytes(data,length);	//再写入bytes 数据
}
//返回bytes的长度
const byte* HByteConvert::readBytes(HDataBuffer &buf, int &size)
{
	const byte *pBuf = buf.fetchBytes(4);		//得到长度数据
	size = byteToInt(pBuf);

	return buf.fetchBytes(size);
}

void HByteConvert::writeShort(HDataBuffer &buf, short data)
{
	byte *pBuf = shortToByte(data);
	buf.putBytes(pBuf, 2);
}

short HByteConvert::readShort(HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(2);
	short t = byteToShort(pBuf);
	return t;
}

void HByteConvert::writeInt(HDataBuffer &buf, int data)
{
	byte *pBuf = intToByte(data);
	buf.putBytes(pBuf, 4);
}

int HByteConvert::readInt(HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(4);
	int t = byteToInt(pBuf);
	return t;
}

void HByteConvert::writeDouble(HDataBuffer &buf, double &data)
{
const byte *pBuf = doubleToByte(data);
// 	int16 len = strlen((char *)pBuf);
//
// 	byte *pBuf_ = shortToByte(len);//写入长度
// 	buf.putBytes(pBuf_, 2);
// 	delete[]pBuf_;

buf.putBytes(pBuf, 8);//写入数据
}

double HByteConvert::readDouble(HDataBuffer &buf)
{
const byte *pBuf = buf.fetchBytes(8);
// 	uint16 len = byteToShort(pBuf);//获取长度
// 	delete []pBuf;
//
// 	if (len == 0) return 0;
//
// 	pBuf = buf.fetchBytes(len);
double ret = byteToDouble(pBuf);
return ret;
}
void HByteConvert::writeLong64(HDataBuffer &buf, long long data)
{
	byte *pBuf = long64ToByte(data);
	buf.putBytes(pBuf, 8);
}

long long HByteConvert::readLong64(HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(8);
	long long t = byteToLong64(pBuf);
	return t;
}
*/
/*
void HByteConvert::write256String(HDataBuffer &buf, string &data)
{
	int16 len = data.length();
	len >= 255 ? len = 255 : 0;
	byte l = (byte)len;
	buf.putBytes(&l,1);//写入string长度
	byte *pBuf = stringToByte(data);
	buf.putBytes(pBuf, len);
	delete[]pBuf;
}

void HByteConvert::write256String(HDataBuffer &buf, QString &data)
{
	int16 len = data.size();
	len >= 255 ? len = 255 : 0;
	byte l = (byte)len;
	buf.putBytes(&l, 1);
	byte *pBuf = stringToByte(data);
	buf.putBytes(pBuf, len);
	delete[]pBuf;
}

void HByteConvert::read256String(string &_256String, HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(1);//得到长度
	int len = pBuf[0];

	if (len == 0)
	{
		_256String = "";
		return;
	}
	pBuf = buf.fetchBytes(len);
	byteToString(_256String, pBuf);
}

void HByteConvert::read256String(QString &_256String, HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(1);//得到长度
	int len = pBuf[0];

	if (len == 0)
	{
		_256String = "";
		return;
	}
	pBuf = buf.fetchBytes(len);
	byteToString(_256String, pBuf);
}

void HByteConvert::writeShortString(HDataBuffer &buf, string &data)
{
	int32 len = data.length();
	len > 65535 ? len = 65535 : 0;

	const byte *pBuf = shortToByte((uint16)len);
	buf.putBytes(pBuf, 2);

	pBuf = stringToByte(data);
	buf.putBytes(pBuf, (uint16)len);
	delete[]pBuf;
}

void HByteConvert::writeShortString(HDataBuffer &buf, QString &data)
{
	int32 len = data.length();
	len > 65535 ? len = 65535 : 0;

	byte *pBuf = shortToByte((uint16)len);
	buf.putBytes(pBuf, 2);

	pBuf = stringToByte(data);
	buf.putBytes(pBuf, (uint16)len);
	delete[]pBuf;
}

void HByteConvert::readShortString(string &_shortString, HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(2);//得到长度
	uint16 len = byteToShort(pBuf);

	if (len == 0)
	{
		_shortString = "";
		return;
	}
	pBuf = buf.fetchBytes(len);
	byteToString(_shortString, pBuf);
}

void HByteConvert::readShortString(QString &_shortString, HDataBuffer &buf)
{
	const byte *pBuf = buf.fetchBytes(2);//得到长度
	uint16 len = byteToShort(pBuf);

	if (len == 0)
	{
		_shortString = "";
		return;
	}
	pBuf = buf.fetchBytes(len);
	byteToString(_shortString, pBuf);
}

void HByteConvert::writeVectorString(HDataBuffer &buf, vector<string> &arry)
{
	int num = arry.size();
	const byte *pBuf = shortToByte((uint16)num);
	buf.putBytes(pBuf, 2);

	vector<string>::iterator iter = arry.begin();
	while (iter != arry.end())
	{
		writeShortString(buf,*iter);
		++iter;
	}
}

void HByteConvert::readVectorString(HDataBuffer &buf, vector<string> &result)
{
	const byte *pBuf = buf.fetchBytes(2);//得到vector的个数
	uint16 len = byteToShort(pBuf);

	for (uint16 i = 0; i < len; i++)
	{
		string str;
		readShortString(str, buf);
		result.push_back(str);
	}
}
*/
NAMESPACE_END_CHE