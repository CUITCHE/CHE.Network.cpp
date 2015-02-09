#ifndef HByteConvert_h__
#define HByteConvert_h__
/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/08
	created:	8:2:2014   19:36
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\HByteConvert.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	HByteConvert
	file ext:	h
	author:		Jq
	
	purpose:	为了提高转换函数的效率，每个转换函数都共享一个union _DATATYPESET数据类型集
				所以这个转换函数不是线程安全的，请不要在多线程下使用它
*********************************************************************/
#include "HDataBuffer.h"
#include "HCodeConversion.hpp"
#include <string>

using std::wstring;
using std::string;

class HDataBuffer;
NAMESPACE_BEGIN_CHE
#ifndef DATATYPESET
typedef union _DATATYPESET{
	_DATATYPESET() :_int64_(0){}
	_DATATYPESET(const bool rVal) :_int64_(0){ _bool = rVal; }
	_DATATYPESET(const byte rVal) :_byte({ { rVal, 0, 0, 0, 0, 0, 0, 0 } }){}
	_DATATYPESET(const char rVal) :_byte({ { (const byte)rVal, 0, 0, 0, 0, 0, 0, 0 } }){}
	_DATATYPESET(const short rVal) :_short({ rVal, 0, 0, 0 }){}
	_DATATYPESET(const int rVal) :_int_({ rVal, 0 }){}
	_DATATYPESET(const float rVal) :_float({ rVal, 0 }){}
	_DATATYPESET(const double &rVal) :_double(rVal){}
	_DATATYPESET(const long long &rVal) :_int64_(rVal){}

	H_INLINE void operator=(const bool rVal){ _bool = rVal; }
	H_INLINE void operator=(const byte rVal){ _byte = rVal; }
	H_INLINE void operator=(const short rVal){ _short = rVal; }
	H_INLINE void operator=(const int rVal){ _int_ = rVal; }
	H_INLINE void operator=(const float rVal){ _float = rVal; }
	H_INLINE void operator=(const double &rVal){ _double = rVal; }
	H_INLINE void operator=(const long long &rVal){ _int64_ = rVal; }

	H_INLINE operator bool()const{ return _bool; }
	H_INLINE operator byte*()const{ return _byte; }
	H_INLINE operator byte()const{ return _byte._1st[0]; }
	H_INLINE operator uint16()const{ return (uint16)_short._1st; }
	H_INLINE operator short()const{ return _short; }
	H_INLINE operator uint32()const{ return (uint32)_int_; }
	H_INLINE operator int()const{ return _int_; }
	H_INLINE operator float()const{ return _float; }
	H_INLINE operator double()const{ return _double; }
	H_INLINE operator long long()const{ return _int64_; }
	H_INLINE byte& operator[](const int pos){ return _byte._1st[pos]; }

	struct _1{
		mutable byte _1st[8];
		H_INLINE void operator=(const byte rVal){ _1st[0] = rVal; }
		H_INLINE void operator=(const char rVal){ _1st[0] = (byte)rVal; }
		H_INLINE operator byte*()const{ return _1st; }
		H_INLINE operator char*()const{ return (char*)_1st; }
	}_byte;
	struct _2{
		short _1st;
		short _2st;
		short _3st;
		short _4st;
		H_INLINE void operator=(const short rVal){ _1st = rVal; }
		H_INLINE operator short()const{ return _1st; }
	}_short;
	struct _3{
		int _1st;
		int _2st;
		H_INLINE void operator=(const int rVal){ _1st = rVal; }
		H_INLINE operator int()const{ return _1st; }
	}_int_;
	struct _4{
		float _1st;
		float _2st;
		H_INLINE void operator=(const float rVal){ _1st = rVal; }
		H_INLINE operator float()const{ return _1st; }
	}_float;
	struct _5{
		bool _1st[64];
		H_INLINE void operator=(const bool rVal){ _1st[0] = rVal; }
		H_INLINE operator bool()const{ return _1st[0]; }
	}_bool;
	long long _int64_;
	double _double;
}DATATYPESET;
#endif // !DATATYPESET

class HByteConvert
{
public:
	HByteConvert() = delete;
	~HByteConvert() = delete;
	HByteConvert & operator=(const HByteConvert &) = delete;
	HByteConvert(const HByteConvert &) = delete;

	/*ws2s：wstring转string s2ws：string转wstring*/
	static string ws2s(const wstring &ws);
	static wstring s2ws(const string &s);
public:
	/*接口函数定义*/
	template < typename T, bool bool_t = (sizeof(T) != sizeof(string) && sizeof(T) != sizeof(wstring)) > 
	H_INLINE static void write(HDataBuffer &buf, const T &data){
		write_t(buf, data, ObjectdefsPrivate::bool_trial<bool_t>());
	}
	template < typename T, bool bool_t = (sizeof(T) != sizeof(string) && sizeof(T) != sizeof(wstring)) >
	H_INLINE static void read(T &data, HDataBuffer &buf){
		read_t(buf, data, ObjectdefsPrivate::bool_trial<bool_t>());
	}

	/*模版转换函数，只支持内置数据类型bool, short, int, float, double, long long的转换*/
	template<typename T>
	H_INLINE static const byte* toBytes(const T &value){
		_convertDataTypeCollection = value;
		return _convertDataTypeCollection;
	}
	template<typename T>
	H_INLINE static T toType(const byte *pBuf){
		memcpy<T>(_convertDataTypeCollection, pBuf);
		return _convertDataTypeCollection;
	}
	/*向HDataBuffer中写入字节数据*/

	//写入非string or wstring 数据
	template<typename T>
	H_INLINE static void write_t(HDataBuffer &buf, const T &data, ObjectdefsPrivate::bool_trial<true>){
		const int length = sizeof(T);
		const byte *pBuf = toBytes(data);
		buf.putBytes(pBuf, length);
	}
	template<typename T>
	H_INLINE static T read_t(HDataBuffer &buf, ObjectdefsPrivate::bool_trial<true>){
		const int length = sizeof(T);
		const byte *pBuf = buf.fetchBytes(length);
		return toType<T>(pBuf);
	}
	template<typename T>
	H_INLINE static void read_t(HDataBuffer &buf, T &data, ObjectdefsPrivate::bool_trial<true> bt){
		data = read_t<T>(buf, bt);
	}

	//写入string or wstring 数据
	template<typename T>
	H_INLINE static void write_t(HDataBuffer &buf, const T &data, ObjectdefsPrivate::bool_trial<false>){
		write_str(buf, data);
	}
	template<typename T>
	H_INLINE static T read_t(HDataBuffer &buf, ObjectdefsPrivate::bool_trial<false>){
		return read_str<T>(buf);
	}
	template<typename T>
	H_INLINE static void read_t(HDataBuffer &buf, T &data, ObjectdefsPrivate::bool_trial<false> bf){
		data = read_t<T>(buf, bf);
	}
	/*string - write & read*/
	H_INLINE static void write_str_t(HDataBuffer &buf, const wstring &wstr){
		const wchar_t *buffer = wstr.c_str();
		const int len = wstr.length();
		byte *pbUTF8 = new byte[len + 1];
		CheZeroMemory(pbUTF8, len + 1);

		int writeInLen = HCodeConversion::UTF16Str_To_UTF8Str((WORD *)buffer, (byte *)pbUTF8);
		//写入长度
		write(buf, writeInLen);
		//写入字符串utf8格式byte[]
		buf.putBytes(pbUTF8, writeInLen);
		delete []pbUTF8;
	}
	H_INLINE static void write_str_t(HDataBuffer &buf, const string &str){
		wstring wstr = s2ws(str);
		write_str_t(buf, wstr);
	}
	
	H_INLINE static void read_str_t(wstring &wstr, HDataBuffer &buf){
		//获取长度
		int len = read_t<int>(buf,ObjectdefsPrivate::bool_trial<true>());
		if (len == 0){
			wstr.clear();
			return;
		}
		const byte *pbUTF8 = buf.fetchBytes(len);
		wchar_t *pwChar = new wchar_t[len + 1];

		int len16 = HCodeConversion::UTF8Str_To_UTF16Str(pbUTF8, (WORD *)pwChar);
		pwChar[len16] = 0;
		wstr = pwChar;
		delete []pwChar;
	}
	H_INLINE static void read_str_t(string &data, HDataBuffer &buf){
		wstring wstr;
		read_str_t(wstr, buf);
		data = ws2s(wstr);
	}

	template<typename T>
	H_INLINE static void write_str(HDataBuffer &buf, const T &str){
		write_str_t(buf, str);
	}

	template<typename T>
	H_INLINE static T read_str(HDataBuffer &buf){
		T str;
		read_str_t(str, buf);
		return str;
	}
	/*	static const byte* shortToByte(const short data);
	static void shortToByte(const short data, byte *pBuf);
	static short byteToShort(const byte *pBuf);

	static byte* intToByte(const int data);
	static void intToByte(const int data, byte *pBuf);
	static int byteToInt(const byte *pBuf);

	static byte* long64ToByte(const long long &data);
	static void long64ToByte(const long long &data, byte *pBuf);
	static long long byteToLong64(const byte *pBuf);
	*/
	/*
	static  byte* doubleToByte(const double &data);
	static  void doubleToByte(const double &data, byte *&pBuf);
	static  double byteToDouble(const byte *pBuf);

	static  byte* floatToByte(const float data);
	static  void floatToByte(const float data, byte *pBuf);
	static  float byteToFloat(const byte *pBuf);
	*/
/*
	static void writeByte(HDataBuffer &buf, byte data);
	static byte readByte(HDataBuffer &buf);

	static void writeBytes(HDataBuffer &buf, byte *data, INT length);
	static const byte* readBytes(HDataBuffer &buf, INT &size);

	static void writeShort(HDataBuffer &buf, short data);
	static short readShort(HDataBuffer &buf);

	static void writeInt(HDataBuffer &buf, int data);
	static int readInt(HDataBuffer &buf);

	//写入float或者double都行
	static void writeDouble(HDataBuffer &buf, double &data);
	static double readDouble(HDataBuffer &buf);

	static void writeLong64(HDataBuffer &buf, long long data);
	static long long readLong64(HDataBuffer &buf);
*/
	//最大只能写入255个ASCII字符
/*
	static void write256String(HDataBuffer &buf, string &data);
	static void read256String(string &_256String, HDataBuffer &buf);

	static void write256String(HDataBuffer &buf, QString &data);
	static void read256String(QString &_256String, HDataBuffer &buf);
	//最大只能写入65535个ASCII字符
	static void writeShortString(HDataBuffer &buf, string &data);
	static void readShortString(string &_shortString, HDataBuffer &buf);

	static void writeShortString(HDataBuffer &buf, QString &data);
	static void readShortString(QString &_shortString, HDataBuffer &buf);

	static void writeVectorString(HDataBuffer &buf, vector<string> &arry);
	static void readVectorString(HDataBuffer &buf,  vector<string> &result);
	*/
protected:
	template<typename T>
	H_INLINE static void memcpy(void *_dst, const void *_src){ T *dst = (T *)_dst; T *src = (T *)_src; *dst++ = *src++; }
private:
	static DATATYPESET _convertDataTypeCollection;			//数据类型集
};
NAMESPACE_END_CHE
#endif // HByteConvert_h__