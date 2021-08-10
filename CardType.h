#pragma once
#ifndef _CARDTYPE_H_
#define _CARDTYPE_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
class CCardType
{
public:
	CCardType();/*���캯��*/
	~CCardType();/*��������*/
	bool Init(mysqlpp::Row& row);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	const std::string& getName() const;/*��ȡm_strName*/
	unsigned int getId() const;/*��ȡm_unId*/
	unsigned int getCardType() const;/*��ȡm_unCardType*/
	long long int getHp() const;/*��ȡm_i64Hp*/
	long long int getMp() const;/*��ȡm_i64Mp*/
	long long int getAtk() const;/*��ȡm_i64Atk*/
private:
	unsigned int m_unId=0,m_unCardType=0;
	std::string m_strName="";
	long long int m_i64Hp=0, m_i64Mp=0, m_i64Atk=0;
};
#endif //!_CARDTYPE_H_