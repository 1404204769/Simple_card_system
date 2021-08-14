#pragma once
#ifndef _SKINTYPE_H_
#define _SKINTYPE_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CSkinType
{
public:
	CSkinType();/*���캯��*/
	~CSkinType();/*��������*/

	bool Init(const mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/

	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetId() const;/*��ȡm_unId*/
	long long int GetHp() const;/*��ȡm_i64Hp*/
	long long int GetMp() const;/*��ȡm_i64Mp*/
	long long int GetAtk() const;/*��ȡm_i64Atk*/

private:
	unsigned int m_unId = 0;
	long long int m_i64Hp = 0, m_i64Mp = 0, m_i64Atk = 0;
	std::string m_strName;
};
#endif //!_SKINTYPE_H_
