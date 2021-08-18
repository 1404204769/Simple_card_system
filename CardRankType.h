#pragma once
#ifndef _CARDRANKTYPE_H_
#define _CARDRANKTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardRankType
{
public:
	CCardRankType();/*���캯��*/
	~CCardRankType();/*��������*/

	bool Init(const mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬���ƽ׼���������*/

	unsigned int GetId() const;/*��ȡm_unId*/
	unsigned int GetCostCardNum() const;/*��ȡm_unCostCardNum*/
	long long int GetHp() const;/*��ȡm_i64Hp*/
	long long int GetMp() const;/*��ȡm_i64Mp*/
	long long int GetAtk() const;/*��ȡm_i64Atk*/

private:
	unsigned int m_unId = 0, m_unCostCardNum = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
};
#endif //!_CARDRANKTYPE_H_
