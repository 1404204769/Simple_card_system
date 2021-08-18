#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include "DB.h"
#include "Card.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "CardTypeMgr.h"
class CUser;
class CCardMgr
{
public:
	CCardMgr();/*构造函数*/
	~CCardMgr();/*析构函数*/

	bool Init(CUser* pUser);/*函数内部连接数据库获取数据*/

	CCard* Get(const long long int i64CardId);/*根据卡牌名称获取卡牌数据*/

	bool Add(const CCardType* pCardType);/*增加一张指定类型的卡牌*/
	bool Del(long long int i64CardId);/*根据玩家卡牌名称来删除数据*/
	bool DelAll();/*删除所有的卡牌*/
	void PrintAll();/*打印显示拥有的所有卡牌的数据*/
	bool IsInit()const;/*检查是否初始化*/

private:
	
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	CUser* m_pUser = nullptr;
	bool m_bInit = false;

	using CardMap = std::map<long long int, CCard*>;
	using CardMapIter = CardMap::iterator;
	CardMap m_mapByCardId;/*每一个都代表目前已有的卡牌类型*/
};

#endif //!_CARDMGR_H_
