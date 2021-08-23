#pragma once
#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "EquipType.h"
class CCard;
class CEquip
{
public:
	CEquip();/*构造函数*/
	~CEquip();/*析构函数*/

	bool CreateNewEquip(const long long int i64UserId, const CEquipType* pEquipType);/*构造一个新的装备*/
	bool CreateFromDB(const mysqlpp::Row& row, const CEquipType* pEquipType);/*从数据库中加载装备*/

	unsigned int GetEquipType() const;/*获取m_unEquipType*/
	long long int GetEquipId() const;/*获取m_i64CardId*/
	long long int GetUserId() const;/*获取m_i64UserId*/

	bool Delete();/*将Card数据从数据库删除*/
private:
	bool Insert();/*将Card数据插入数据库*/
	//bool Update();/*将最新的数据更新到数据库*/
	unsigned int  m_unEquipType = 0;
	long long int m_i64EquipId = 0, m_i64UserId = 0;
};
#endif //!_EQUIP_H_
