#pragma once
#ifndef _CARDLEVATTRTYPE_H_
#define _CARDLEVATTRTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardLevAttrType
{
public:
	CCardLevAttrType();/*构造函数*/
	~CCardLevAttrType();/*析构函数*/

	bool Init(const mysqlpp::Row& row);/*根据获取的数据库数据初始化静态卡牌等级属性数据*/

	unsigned int GetId() const;/*获取m_unId*/
	unsigned int GetCardLev() const;/*获取m_unCardType*/
	long long int GetHp() const;/*获取m_i64Hp*/
	long long int GetMp() const;/*获取m_i64Mp*/
	long long int GetAtk() const;/*获取m_i64Atk*/

private:
	unsigned int m_unId = 0, m_unLev = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
};
#endif //!_CARDLEVATTRTYPE_H_
