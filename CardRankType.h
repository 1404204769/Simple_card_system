#pragma once
#ifndef _CARDRANKTYPE_H_
#define _CARDRANKTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardRankType
{
public:
	CCardRankType();/*构造函数*/
	~CCardRankType();/*析构函数*/

	bool Init(const mysqlpp::Row& row);/*根据获取的数据库数据初始化静态卡牌阶级属性数据*/

	unsigned int GetId() const;/*获取m_unId*/
	unsigned int GetCostCardNum() const;/*获取m_unCostCardNum*/
	long long int GetHp() const;/*获取m_i64Hp*/
	long long int GetMp() const;/*获取m_i64Mp*/
	long long int GetAtk() const;/*获取m_i64Atk*/

private:
	unsigned int m_unId = 0, m_unCostCardNum = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
};
#endif //!_CARDRANKTYPE_H_
