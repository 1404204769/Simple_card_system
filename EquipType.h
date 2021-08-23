#pragma once
#ifndef _EQUIPTYPE_H_
#define _EQUIPTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CEquipType
{
public:
	CEquipType();/*���캯��*/
	~CEquipType();/*��������*/

	bool Init(const mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬װ������*/

	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetId() const;/*��ȡm_unId   0��ʾ���ɴ�����1��ʾñ�ӣ�2��ʾ�¼ף�3��ʾЬ��*/
	unsigned int GetPos() const;/*��ȡm_unPos*/
	long long int GetHp() const;/*��ȡm_i64Hp*/
	long long int GetMp() const;/*��ȡm_i64Mp*/
	long long int GetAtk() const;/*��ȡm_i64Atk*/

private:
	unsigned int m_unId = 0;
	enum Pos { Prohibit=0,Hat,Armour,Shoes} m_unPos = Prohibit;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
	std::string m_strName;
};
#endif //!_EQUIPTYPE_H_
