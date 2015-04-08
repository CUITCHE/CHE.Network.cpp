#ifndef HObject_conn_H__
#define HObject_conn_H__

/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
	All rights reserved.
	Contact: 	CHE (hejun_753@163.com)
	created:	2014/10/28 17:41
	author:		CHE

	purpose:	������źŲ�ģʽ��
	��������ֻ��һ��void*���ͣ����Բ������Ͱ�ȫ�ģ�
	���ݵ�ʱ�����ж�����������������ṹ�崫��
	�ź���۵Ĺ��������sender��meta��
	HObject����public����Ȩ�޵�HMetaObject��ͨ������ϵ�ź���ۣ��źŲ�
	���������а�ȫ��飬�����������Ϣ�Ŀ���̨
	*********************************************************************/
#include "hglobal.h"
#include "HObjectdefs_conn.h"
//#include "HObject.h"
#include <list>
#include <memory>
#include <algorithm>
using std::map;
using std::list;
using std::shared_ptr;

CHE_NAMESPACE_BEGIN
class HObject;
class HMetaSlotObject
{
public:
	typedef void(__stdcall*FuncCall)(void *);
	typedef uint32 FuncType;

	HMetaSlotObject(FuncType func)
		:_func(func)
		, _authorization(0){}
	~HMetaSlotObject(){}

	/*��Ӳۺ��������ظòۺ���������������ӹ������źŵ�Ψһ��
		����0��ʾ�ܾ�����
		*/
	inline int connect(){
		if ((_authorization & 0x80000000) != 0)		return 0;
		if (_authorization == 0)	{ _authorization = 1; return 1; }
		int old = _authorization;
		_authorization <<= 1;
		return (old & _authorization);
	}

	inline void call(HObject *this_, void **arg){
		uint32 This = (uint32)this_;
		FuncCall slotcall = (FuncCall)_func;
		//׼��αthisָ��
		__asm
		{
			mov ecx, This
		};
		//�ۺ������ô�
		slotcall(arg);
	}

	inline bool authorization(int uinqueFlag){
		return (pop(uinqueFlag) == 1 && (uinqueFlag & _authorization));
	}

	template<typename T = uint32, typename Type = ObjectdefsPrivate::RemoveRef<T>::Type>
	inline static Type pop(T x){
		uint64 y = x * 0x0002000400080010ull;
		y &= 0x1111111111111111ull;
		y *= 0x1111111111111111ull;
		y >>= 60;
		return (Type)y;
	}
	FuncType _func;
	int _authorization;
};

struct Connection{
	typedef uint32 FuncType;
	HObject *receiver;
	Connection *first;		//ָ�򱾽�����ĵ�һ��
	Connection *last;		//ָ�򱾽���������һ���ǿս��
	shared_ptr<HMetaSlotObject> slotObject;
	shared_ptr<Connection> nextConnection;			//ָ����һ���źŹ����������źŻ��߲�

	Connection(FuncType func, HObject *receiver) : receiver(receiver), nextConnection(nullptr), last(nullptr),first(nullptr){}
	static void call(Connection *c, void ** arg){
		HObject *recer = c->receiver;
		if (recer == nullptr){
			Warning("receiver can not be nullptr, please check it!");
			return;
		}
		c->slotObject->call(recer, arg);
	}
};

class HMetaObject
{
//	CHE_DISABLE_COPY(HMetaObject);
public:
	typedef void(HObject::*Func)(void *);
	typedef HObject Object;
	//�ź����Ͷ���
	typedef uint32 FuncType;

	HMetaObject(HObject *handler):handler(handler){}
	~HMetaObject(){}

	/*�źŲ۹��� sender must be equal metaObj's handler*/
	static void connect_impl(shared_ptr<HMetaObject> &metaObj, HObject *receiver, FuncType signal, shared_ptr<HMetaSlotObject> slot){
		map<FuncType, shared_ptr<Connection>> &_map = metaObj->signal_slots;
		shared_ptr<Connection> tmp_conn(new Connection(signal, receiver));
		tmp_conn->slotObject = slot;

		map<FuncType, shared_ptr<Connection>>::iterator iter = _map.find(signal);
		if (iter == _map.end()){
			tmp_conn->first = tmp_conn->last = tmp_conn.get();
			_map.insert(std::pair<FuncType, shared_ptr<Connection>>(signal, tmp_conn));
		}
		else {
			Connection *last = iter->second->last;
			last->nextConnection = tmp_conn;
			last = tmp_conn.get();
		}

	}

	/*�ź�ע��*/
	static bool RegSignal(shared_ptr<HMetaObject> &metaObj, FuncType func){
		auto iter = std::find(metaObj->_signals.begin(), metaObj->_signals.end(), func);
		if (iter == metaObj->_signals.end()){
			metaObj->_signals.push_back(func);
			return true;
		}
		Warning("% signal has been definited Duplicatly", func);
		return false;
	}

	static void active(shared_ptr<HMetaObject> &metaObj, FuncType func, void **arg){
		
		list<FuncType>::iterator iter = std::find(metaObj->_signals.begin(), metaObj->_signals.end(), func);
		if (iter == metaObj->_signals.end()){
			Warning("The signal has not been registered!");
			return;
		}

		map<FuncType, shared_ptr<Connection>>::iterator iter_f = metaObj->signal_slots.find(func);

		//���ź�û�б�����
		if (iter_f == metaObj->signal_slots.end())	return;

		metaObj->activePrivate(iter_f->second.get(), arg);
	}
	void activePrivate(Connection *conn_func, void **arg)const{
		Connection::call(conn_func, arg);
		auto next = conn_func->nextConnection;
		//�ź� 1:n
		while (next){
			Connection::call(conn_func, arg);
			next = next->nextConnection;
		}
	}

	list<FuncType> _signals;
	map<FuncType, shared_ptr<Connection>> signal_slots;

	HObject *handler;
};
template<typename FuncAddr>
union U_FuncAddr
{
	DWORD addr;
	FuncAddr func;
};

//�ú궨�������źŶ����д�������ڼ���ۺ�����д�������ļ�(.h)���߶����ļ���(.cpp)����
#define CHE_SIGNAL_DEFINITION(Object,FuncName) void Object::FuncName(void **arg){\
	U_FuncAddr<void(Object::*)(void **arg)> f;\
	f.func = &Object::FuncName;\
	HMetaObject::active(*this->metaObject, f.addr, arg); }
CHE_NAMESPACE_END
#endif // HObject_conn_H__
