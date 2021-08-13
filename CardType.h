#pragma once
#ifndef _CARDTYPE_H_
#define _CARDTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardType
{
public:
	CCardType();/*���캯��*/
	~CCardType();/*��������*/

	bool Init(const mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/

	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetId() const;/*��ȡm_unId*/
	unsigned int GetCardType() const;/*��ȡm_unCardType*/
	long long int GetHp() const;/*��ȡm_i64Hp*/
	long long int GetMp() const;/*��ȡm_i64Mp*/
	long long int GetAtk() const;/*��ȡm_i64Atk*/

private:
	unsigned int m_unId = 0, m_unCardType = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
	std::string m_strName;
};
#endif //!_CARDTYPE_H_
