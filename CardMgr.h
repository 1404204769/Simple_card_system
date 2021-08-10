#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
#include "Card.h"
#include "CardTypeMgr.h"
extern CDB g_DB;
extern CCardTypeMgr g_CardTypeMgr;
class CCardMgr
{
public:
	CCardMgr();/*构造函数*/
	~CCardMgr();/*析构函数*/
	bool Init(long long int _i64UserId);/*函数内部连接数据库获取数据*/
	CCard* GetCardByCardName(std::string _strName);/*根据卡牌名称获取卡牌数据*/
	void PrintAllCard();/*打印显示拥有的所有卡牌的数据*/
	bool AddCard(long long int _i64UserId,const CCardType* _pCardType);/*增加一张指定类型的卡牌*/
	bool DelCardByName(std::string _strName);/*根据玩家卡牌名称来删除数据*/
	bool DelAllCard();/*删除所有的卡牌*/
private:
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	using MapByCardName = std::map<std::string, CCard*>;
	using MapByCardNameIter = MapByCardName::iterator;
	MapByCardName m_mapByCardName;/*每一个都代表目前已有的卡牌类型*/
};
#endif //!_CARDMGR_H_
