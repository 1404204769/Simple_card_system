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
	CEquip();/*���캯��*/
	~CEquip();/*��������*/

	bool CreateNewEquip(const long long int i64UserId, const CEquipType* pEquipType);/*����һ���µ�װ��*/
	bool CreateFromDB(const mysqlpp::Row& row, const CEquipType* pEquipType);/*�����ݿ��м���װ��*/

	unsigned int GetEquipType() const;/*��ȡm_unEquipType*/
	long long int GetEquipId() const;/*��ȡm_i64CardId*/
	long long int GetUserId() const;/*��ȡm_i64UserId*/

	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/
private:
	bool Insert();/*��Card���ݲ������ݿ�*/
	//bool Update();/*�����µ����ݸ��µ����ݿ�*/
	unsigned int  m_unEquipType = 0;
	long long int m_i64EquipId = 0, m_i64UserId = 0;
};
#endif //!_EQUIP_H_
