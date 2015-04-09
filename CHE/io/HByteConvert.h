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

CHE_NAMESPACE_BEGIN
#ifndef DATATYPESET
typedef union _DATATYPESET{
	_DATATYPESET() :_int64_(0){}
	_DATATYPESET(const bool rVal) :_int64_(rVal){ }
	_DATATYPESET(const byte rVal) :_int64_(rVal){}
	_DATATYPESET(const char rVal) :_int64_(rVal){}
	_DATATYPESET(const short rVal) :_int64_(rVal){}
	_DATATYPESET(const int rVal) :_int64_(rVal){}
	_DATATYPESET(const float rVal) :_double(rVal){}
	_DATATYPESET(const double &rVal) :_double(rVal){}
	_DATATYPESET(const long long &rVal) :_int64_(rVal){}

	inline void operator=(const bool rVal){ _int64_ = rVal; }
	inline void operator=(const byte rVal){ _int64_ = rVal; }
	inline void operator=(const short rVal){ _int64_ = rVal; }
	inline void operator=(const int rVal){ _int64_ = rVal; }
	inline void operator=(const float rVal){ _float._1st = rVal; }
	inline void operator=(const double &rVal){ _double = rVal; }
	inline void operator=(const long long &rVal){ _int64_ = rVal; }

	inline operator bool()const{ return _bool; }
	inline operator byte*()const{ return _byte; }
	inline operator byte()const{ return _byte._1st[0]; }
	inline operator uint16()const{ return (uint16)_short; }
	inline operator short()const{ return _short; }
	inline operator uint32()const{ return (uint32)_int_; }
	inline operator int()const{ return _int_; }
	inline operator float()const{ return _float; }
	inline operator double()const{ return _double; }
	inline operator long long()const{ return _int64_; }
	inline byte& operator[](const int pos){ return _byte._1st[pos]; }

	struct _1{
		mutable byte _1st[8];
		inline void operator=(const byte rVal){ _1st[0] = rVal; }
		inline void operator=(const char rVal){ _1st[0] = (byte)rVal; }
		inline operator byte*()const{ return &_1st[0]; }
		inline operator char*()const { return (char*)&_1st[0]; }
	}_byte;
	struct _2{
		short _1st;
		short _2st;
		short _3st;
		short _4st;
		inline void operator=(const short rVal){ _1st = rVal; }
		inline operator short()const{ return _1st; }
	}_short;
	struct _3{
		int _1st;
		int _2st;
		inline void operator=(const int rVal){ _1st = rVal; }
		inline operator int()const{ return _1st; }
	}_int_;
	struct _4{
		float _1st;
		float _2st;
		inline void operator=(const float rVal){ _1st = rVal; }
		inline operator float()const{ return _1st; }
	}_float;
	struct _5{
		bool _1st[8];
		inline void operator=(const bool rVal){ _1st[0] = rVal; }
		inline operator bool()const { return _1st[0];}
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

	/*
	ws2s：wstring转string 
	s2ws：string转wstring
	*/
	static string ws2s(const wstring &ws);
	static wstring s2ws(const string &s);
public:
	/*接口函数定义*/
	template < typename T> 
	inline static void write(HDataBuffer &buf, const T &data){
		write_t(buf, data);
	}
	template < typename T>
	inline static void read(T &data, const HDataBuffer &buf){
		read_t(buf, data);
	}
	//自定义write方法。
	//用于要写入自定义类
	template <typename T>
	inline static void write(void(*writeFunc)(void) ) {
		writeFunc();
	}
	//自定义read方法。
	//用于要读入自定义类
	template < typename T>
	inline static void read(std::function<void(void)> &readFunc) {
		readFunc();
	}
	/*模版转换函数，只支持内置数据类型bool, short, int, float, double, long long的转换*/
	template<typename T>
	inline static const byte* toBytes(const T &value){
		_convertDataTypeCollection = value;
		return (byte *)_convertDataTypeCollection + sizeof(long long) % sizeof(T);
	}
	template<typename T>
	inline static T toType(const byte *pBuf){
		memcpy<T>(_convertDataTypeCollection, pBuf);
		auto p = &_convertDataTypeCollection;
		return (T)_convertDataTypeCollection;
	}
protected:
#pragma region 字节化POD 数据
	template<typename T>
	inline static void write_t(HDataBuffer &buf, const T &data){
		const int length = sizeof(T);
		const byte *pBuf = toBytes(data);
		buf.putBytes(pBuf, length);
	}
	template<typename T>
	inline static T read_t(const HDataBuffer &buf){
		const int length = sizeof(T);
		const byte *pBuf = buf.fetchBytes(length);
		return toType<T>(pBuf);
	}
	template<typename T>
	inline static void read_t(const HDataBuffer &buf, T &data){
		const int length = sizeof(T);
		const byte *pBuf = buf.fetchBytes(length);
		data = toType<T>(pBuf);
	}
#pragma endregion

#pragma region string 特化
	template<>
	inline static void write_t<string>(HDataBuffer &buf, const string &data) {
		wstring wstr = s2ws(data);
		write_t(buf, wstr);
	}
	template<>
	inline static void read_t<string>(const HDataBuffer &buf, string &data) {
		wstring wstr;
		read_t(buf, wstr);
		data = ws2s(wstr);
	}
#pragma endregion

#pragma region wstring 特化
	template<>
	inline static void write_t<wstring>(HDataBuffer &buf, const wstring &data) {
//#error do not use wstring to transport data
		wchar_t *buffer = const_cast<wchar_t*>(data.c_str());
		byte *pB = nullptr;
		int nlen = HCodeConversion::UTF16Str_To_UTF8Str((WORD *)buffer, pB);
		//写入长度
		write_t(buf, nlen);
		//写入数据
		buf.putBytes(pB, nlen);
	}
	template<>
	inline static void read_t<wstring>(const HDataBuffer &buf, wstring &data) {
//#error do not use wstring to transport data
		//读取长度
		int nlen = read_t<int>(buf);
		const byte *dst = buf.fetchBytes(nlen+1);
		byte save = dst[nlen];
		const_cast<byte *>(dst)[nlen] = 0;

		WORD *pChar = nullptr;
		int len = HCodeConversion::UTF8Str_To_UTF16Str(dst, pChar);

		const_cast<byte *>(dst)[nlen] = save;
		buf.addReadPosition(-1);
		data.insert(0, (wchar_t *)pChar, len);
	}
#pragma endregion

#pragma region vector 特化
	template<typename T>
	inline static void write_t(HDataBuffer &buf, const vector<T> &data) {
		//写入vector的size
		int size = data.size();
		write_t(buf, size);
		for each(const T &var in data) {
			write_t(buf, var);
		}
	}
	template<typename T>
	inline static void read_t(const HDataBuffer &buf, vector<T> &data) {
		//获得长度
		int size = read_t<int>(buf);
		data.resize(size);
		T _d_;
		for (int i = 0; i < size; ++i) {
			read_t(buf, _d_);
			data.push_back(_d_);
		}
	}
#pragma endregion
protected:
	template<typename T>
	inline static void memcpy(void *_dst, const void *_src){ T *dst = (T *)_dst; T *src = (T *)_src; *dst = *src; }
private:
	static DATATYPESET _convertDataTypeCollection;			//数据类型集
};
CHE_NAMESPACE_END
#endif // HByteConvert_h__