#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
#include "CardType.h"
extern CDB g_DB;
class CCard
{
public:
	CCard();/*构造函数*/
	~CCard();/*析构函数*/
	bool Init(const long long int _i64CardId,const long long int _i64UserId,const unsigned int _unCardType,const std::string& _strName,const long long int _i64Exp,const unsigned int _unLev, const CCardType* _pCardType);/*根据获取的数据库数据初始化静态卡牌数据*/
	std::string& GetName();/*获取m_strName*/
	unsigned int GetLev();/*获取m_unLev*/
	unsigned int GetCardType();/*获取m_unCardType*/
	unsigned int GetMark();/*获取m_unMark*/
	long long int GetCardId();/*获取m_i64CardId*/
	long long int GetUserId();/*获取m_i64UserId*/
	long long int GetExp();/*获取m_i64Exp*/
	bool SetMark(unsigned int _unMark);/*设置栈状态 1.增 2.删 3.改 4.无变化*/
	const CCardType* GetCardTypeData();/*获取对应的卡牌类型详细数据*/

private:
	bool Insert();/*将Card数据插入数据库*/
	bool Delete();/*将Card数据从数据库删除*/
	bool Update();/*更新数据库*/
	unsigned int  m_unCardType = 0, m_unLev = 0,m_unMark=0;
	std::string m_strName = "";
	long long int m_i64CardId = 0, m_i64UserId = 0, m_i64Exp = 0;
	const CCardType* m_pCardType = nullptr;
};
#endif //!_CARD_H_
