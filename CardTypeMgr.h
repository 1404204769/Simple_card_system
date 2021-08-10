#pragma once
#ifndef _CARDTYPEMGR_H_
#define _CARDTYPEMGR_H_
#include "DB.h"
#include <string>
#include <sstream>
#include <mysql++.h>
#include "CardType.h"
extern CDB g_DB;
class CCardTypeMgr
{
public:
	CCardTypeMgr();/*构造函数*/
	~CCardTypeMgr();/*析构函数*/
	const CCardType* GetCardTypeByType(unsigned int _unType);/*根据卡牌ID获取数据*/
	void PrintAllCardType();/*打印显示所有卡牌类型*/
	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/
private:
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	using MapByType = std::map<unsigned int,const CCardType*>;
	using MapByTypeIter = MapByType::iterator;
	MapByType m_mapByType;/*每一个都代表目前已有的卡牌类型*/
};
#endif //!_CARDTYPEMGR_H_
