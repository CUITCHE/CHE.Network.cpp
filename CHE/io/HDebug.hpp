#ifndef HDebug_H__
#define HDebug_H__
//
/********************************************************************
	created:	2014/10/14
	created:	14:10:2014   10:59
	file ext:	hpp
	author:		CHE
	
	purpose:	µ÷ÊÔÊä³öÀà
*********************************************************************/
#include "HByteConvert.h"
#include <string>
#include <ostream>
#include <functional>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <sstream>
using std::set;
using std::map;
using std::list;
using std::vector;
using std::string;
using std::wstring;
using std::function;
using std::multiset;
using std::multimap;
using std::stringstream;

CHE_NAMESPACE_BEGIN
class HDebug
{
	struct Stream{
		Stream(MsgType t):type(t),space(true){}
		MsgType type;
		bool space;
		stringstream ss;
	}*stream;
public:
	inline HDebug(MsgType t):stream(new Stream(t)){}
	HDebug() = delete;
/*	const HDebug& operator=(const HDebug&) = delete;
	const HDebug& operator=(const HDebug&&) = delete;*/
	inline ~HDebug(){
		try{
			string buf = stream->ss.str();
			const char *str = buf.c_str();
			h_message_output(stream->type, str);
		}catch (...){/*ºöÂÔ´íÎó*/}
		delete stream;
	}
	inline HDebug& space(){ stream->space = true; stream->ss << " "; return *this; }
	inline HDebug& noSpace(){ stream->space = false; return *this; }
	inline HDebug& maybeSpace(){ if (stream->space)stream->ss << " "; return *this; }

	inline HDebug& operator<<(const bool t) { stream->ss << (t ? "true" : "false"); return maybeSpace(); }
	inline HDebug& operator<<(const char t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const signed short t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const unsigned short t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const signed int t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const unsigned int t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const signed long t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const unsigned long t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const int64 t){stream->ss <<t; return maybeSpace();}
	inline HDebug& operator<<(const uint64 t){stream->ss << t; return maybeSpace();}
	inline HDebug& operator<<(const float t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const double t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const long double t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const char* t) { stream->ss << t; return maybeSpace(); }
	inline HDebug& operator<<(const string &t){ stream->ss << '\"' << t << '\"'; return maybeSpace(); }
//	inline HDebug& operator<<(const wstring & t) { stream->ss << '\"' << t << '\"'; return maybeSpace();}
	inline HDebug& operator<<(const void * t) { stream->ss << t; return maybeSpace(); }
//	inline HDebug& operator<<(const wchar_t *t){ stream->ss << t; return maybeSpace(); }
};
class HNoDebug
{
public:
	inline HNoDebug(){}
	inline HNoDebug(const HNoDebug&){}
	inline ~HNoDebug(){}
	inline HNoDebug &space() { return *this; }
	inline HNoDebug &nospace() { return *this; }
	inline HNoDebug &maybeSpace() { return *this; }

	template<typename T>
	inline HNoDebug &operator<<(const T &) { return *this; }
};
#ifdef _DEBUG
HDebug hDebug(){ return HDebug(DebugMsg); }
HDebug hWarning(){ return HDebug(WarningMsg); }
#else
HNoDebug hDebug(){ return HNoDebug(); }
#endif // _DEBUG
template<typename T>
inline HDebug& operator<<(HDebug &out, const list<T> &data){
	out.noSpace() << "list(";
	list<T>::const_iterator iter = data.begin();
	if (data.size() > 1){
		out << *iter;
	}
	for (++iter; iter != data.end(); ++iter){
		out << ", " << *iter;
	}
	out << ")";
	return out.space();
}
template<typename T>
inline HDebug& operator<<(HDebug &out, const vector<T> &data){
	out.noSpace() << "vector(";
	vector<T>::const_iterator iter = data.begin();
	if (data.size() > 1){
		out << *iter;
	}
	for (++iter; iter != data.end(); ++iter){
		out << ", " << *iter;
	}
	out << ")";
	return out.space();
}
template<typename aKey, typename aT>
inline HDebug& operator<<(HDebug &out, const map<aKey, aT> &data){
	out.noSpace() << "map{";
	map<aKey, aT>::const_iterator iter = data.begin();
	for (; iter != data.end(); ++iter){
		out << "(" << iter->first << ", " << iter->second << ")";
	}
	out << "}";
	return out.space();
}
template<typename T>
inline HDebug& operator<<(HDebug &out, const set<T> &data){
	out.noSpace() << "set(";
	set<T>::const_iterator iter = data.begin();
	if (data.size() > 1){
		out << *iter;
	}
	for (++iter; iter != data.end(); ++iter){
		out << ", " << *iter;
	}
	out << ")";
	return out.space();
}
template<typename T>
inline HDebug& operator<<(HDebug &out, const multiset<T> &data){
	out.noSpace() << "multiset(";
	multiset<T>::const_iterator iter = data.begin();
	if (data.size() > 1){
		out << *iter;
	}
	for (++iter; iter != data.end(); ++iter){
		out << ", " << *iter;
	}
	out << ")";
	return out.space();
}
template<typename aKey, typename aT>
inline HDebug& operator<<(HDebug &out, const multimap<aKey, aT> &data){
	out.noSpace() << "multimap{";
	multimap<aKey, aT>::const_iterator iter = data.begin();
	for (; iter != data.end(); ++iter){
		out << "(" << iter->first << ", " << iter->second << ")";
	}
	out << "}";
	return out.space();
}
CHE_NAMESPACE_END
#endif // HDebug_H__