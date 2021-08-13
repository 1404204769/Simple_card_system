#pragma once
#ifndef _CARDLEVATTRTYPE_H_
#define _CARDLEVATTRTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardLevAttrType
{
public:
	CCardLevAttrType();/*���캯��*/
	~CCardLevAttrType();/*��������*/

	bool Init(const mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬���Ƶȼ���������*/

	unsigned int GetId() const;/*��ȡm_unId*/
	unsigned int GetCardLev() const;/*��ȡm_unCardType*/
	long long int GetHp() const;/*��ȡm_i64Hp*/
	long long int GetMp() const;/*��ȡm_i64Mp*/
	long long int GetAtk() const;/*��ȡm_i64Atk*/

private:
	unsigned int m_unId = 0, m_unLev = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
};
#endif //!_CARDLEVATTRTYPE_H_
