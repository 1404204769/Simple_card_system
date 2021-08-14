#pragma once
#ifndef _SKINTYPE_H_
#define _SKINTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CSkinType
{
public:
	CSkinType();/*构造函数*/
	~CSkinType();/*析构函数*/

	bool Init(const mysqlpp::Row& row);/*根据获取的数据库数据初始化静态卡牌数据*/

	const std::string& GetName() const;/*获取m_strName*/
	unsigned int GetId() const;/*获取m_unId*/
	long long int GetHp() const;/*获取m_i64Hp*/
	long long int GetMp() const;/*获取m_i64Mp*/
	long long int GetAtk() const;/*获取m_i64Atk*/

private:
	unsigned int m_unId = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
	std::string m_strName;
};
#endif //!_SKINTYPE_H_
