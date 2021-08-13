#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "CardType.h"
class CCard
{
public:
	CCard();/*构造函数*/
	~CCard();/*析构函数*/

	bool CreateNewCard(const long long int i64UserId,const CCardType* pCardType);/*构造一个新的卡牌*/
	bool CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType);/*从数据库中加载卡牌*/

	const std::string& GetName() const;/*获取m_strName*/
	unsigned int GetLev() const;/*获取m_unLev*/
	unsigned int GetCardType() const;/*获取m_unCardType*/
	long long int GetCardId() const;/*获取m_i64CardId*/
	long long int GetUserId() const;/*获取m_i64UserId*/
	long long int GetExp() const;/*获取m_i64Exp*/
	const CCardType& GetCardTypeData() const;/*获取对应的卡牌类型详细数据*/

	bool Delete();/*将Card数据从数据库删除*/

private:
	bool Update();/*更新数据库*/
	bool Insert(long long int& i64CardId_Out);/*将Card数据插入数据库*/

	unsigned int  m_unCardType = 0, m_unLev = 0;
	long long int m_i64CardId = 0, m_i64UserId = 0, m_i64Exp = 0;
	std::string m_strName;   
	const CCardType* m_pCardType = nullptr;
};
#endif //!_CARD_H_
