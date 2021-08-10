#pragma once
#ifndef _CARDTYPE_H_
#define _CARDTYPE_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
class CCardType
{
public:
	CCardType();/*构造函数*/
	~CCardType();/*析构函数*/
	bool Init(mysqlpp::Row& row);/*根据获取的数据库数据初始化静态卡牌数据*/
	const std::string& getName() const;/*获取m_strName*/
	unsigned int getId() const;/*获取m_unId*/
	unsigned int getCardType() const;/*获取m_unCardType*/
	long long int getHp() const;/*获取m_i64Hp*/
	long long int getMp() const;/*获取m_i64Mp*/
	long long int getAtk() const;/*获取m_i64Atk*/
private:
	unsigned int m_unId=0,m_unCardType=0;
	std::string m_strName="";
	long long int m_i64Hp=0, m_i64Mp=0, m_i64Atk=0;
};
#endif //!_CARDTYPE_H_
