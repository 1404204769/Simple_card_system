#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include "DB.h"
#include "Card.h"
#include <vector>
#include <string>
#include <sstream>
#include <mysql++.h>
#include "CardTypeMgr.h"
extern CDB g_DB;
extern CCardTypeMgr g_CardTypeMgr;
class CCardMgr
{
public:
	CCardMgr();/*构造函数*/
	~CCardMgr();/*析构函数*/
	bool Init(long long int _i64UserId);/*函数内部连接数据库获取数据*/
	CCard* GetCardByCardId(long long int _i64CardId);/*根据卡牌名称获取卡牌数据*/
	void PrintAllCard();/*打印显示拥有的所有卡牌的数据*/
	bool AddCard(long long int _i64UserId,const CCardType* _pCardType);/*增加一张指定类型的卡牌*/
	bool DelCardById(long long int _i64CardId);/*根据玩家卡牌名称来删除数据*/
	bool DelAllCard();/*删除所有的卡牌*/
private:
	bool Insert(long long int& _i64CardId, const long long int _i64UserId, const unsigned int _unCardType, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev);/*将Card数据插入数据库*/
	bool Delete(CCard& Card);/*将Card数据从数据库删除*/
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	using MapByCardId = std::map<long long int, CCard*>;
	using MapByCardIdIter = MapByCardId::iterator;
	MapByCardId m_mapByCardId;/*每一个都代表目前已有的卡牌类型*/
	std::vector< CCard*> m_vecDel;
};

#endif //!_CARDMGR_H_
