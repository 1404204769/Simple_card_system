#pragma once
#ifndef _EQUIPTYPE_H_
#define _EQUIPTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CEquipType
{
public:
	CEquipType();/*构造函数*/
	~CEquipType();/*析构函数*/

	bool Init(const mysqlpp::Row& row);/*根据获取的数据库数据初始化静态装备数据*/

	const std::string& GetName() const;/*获取m_strName*/
	unsigned int GetId() const;/*获取m_unId   0表示不可穿戴，1表示帽子，2表示衣甲，3表示鞋子*/
	unsigned int GetPos() const;/*获取m_unPos*/
	long long int GetHp() const;/*获取m_i64Hp*/
	long long int GetMp() const;/*获取m_i64Mp*/
	long long int GetAtk() const;/*获取m_i64Atk*/

private:
	unsigned int m_unId = 0;
	enum Pos { Prohibit=0,Hat,Armour,Shoes} m_unPos = Prohibit;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
	std::string m_strName;
};
#endif //!_EQUIPTYPE_H_
