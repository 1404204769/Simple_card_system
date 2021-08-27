#pragma once
#ifndef _CARDTYPEMGR_H_
#define _CARDTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardType.h"
class CCardTypeMgr
{
public:
	CCardTypeMgr();/*构造函数*/
	~CCardTypeMgr();/*析构函数*/

	const CCardType* Get(const unsigned int unType);/*根据卡牌类型获取数据*/

	void PrintAll();/*打印显示所有卡牌类型*/
	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/

	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
private:

	using CardTypeMap = std::map<unsigned int,const CCardType*>;
	using CardTypeMapIter = CardTypeMap::iterator;
	CardTypeMap m_mapByType;/*每一个都代表目前已有的卡牌类型*/
};
extern CCardTypeMgr g_CardTypeMgr;
#endif //!_CARDTYPEMGR_H_
