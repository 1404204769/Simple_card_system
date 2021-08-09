#pragma once
#ifndef _USER_H_
#define _USER_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
extern CDB g_DB;
class CUser
{
public:
	CUser(const std::string& _strAccount = "wrong",const std::string& _strName = "wrong");
	//CUser(const long long int _i64Id, const std::string& _strAccount, const std::string& _strTime, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev);
	~CUser();/*���û��˳�ʱ���ô�����������������CDB::UpdateData���������ݱ��������ݿ���*/
	const std::string& getAccount();
	const std::string& getCreateTime();
	const std::string& getName();
	const long long int getId();
	const long long int getExp();
	const unsigned int getLev();
	void setLev(const unsigned int _unLev);
	void setExp(const long long int _i64Exp);
	void setName(const std::string& _strName);
	/*
	* ���������ݿ����ؽӿ�
	*/

	bool InitUser(mysqlpp::Row& row);/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	bool InsertUser();/*�����ݿ��в����µ�User����*/
	bool DeleteUser();/*�����ݿ���ɾ��ָ��User����*/
private:
	bool UpdateUser(CUser& pUser);/*�����ݿ��и���ָ��User*/
	unsigned int m_unLev = 0;
	long long int m_i64Id = 0, m_i64Exp = 0;
	std::string m_strAccount = "wrong", m_strCreateTime = "now()", m_strName = "wrong";
};

#endif //!_USER_H_