#pragma once
#ifndef _USER_H_
#define _USER_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardMgr;
class CSkinMgr;
class CEquipMgr;
class CUser
{
public:
	CUser(const std::string& strAccount);
	~CUser();/*���û��˳�ʱ���ô�����������������CDB::UpdateData���������ݱ��������ݿ���*/

	const std::string& GetAccount() const;
	const std::string& GetCreateTime() const;
	const std::string& GetName() const;
	long long int GetId() const;
	long long int GetExp() const;
	unsigned int GetLev() const;
	CCardMgr& GetCardMgr() const;
	CSkinMgr& GetSkinMgr() const;
	CEquipMgr& GetEquipMgr() const;
	void SetLev(const unsigned int unLev);
	void SetExp(const long long int i64Exp);
	void SetName(const std::string& strName);

	/*
	* ���������ݿ����ؽӿ�
	*/

	bool Init(const mysqlpp::Row& row);/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	bool Insert();/*�����ݿ��в����µ�User����*/
	bool Delete();/*�����ݿ���ɾ��ָ��User����*/

private:
	bool Update();/*�����ݿ��и���ָ��User*/

	unsigned int m_unLev = 0;
	long long int m_i64Id = 0, m_i64Exp = 0;
	std::string m_strAccount, m_strCreateTime, m_strName;
	CCardMgr* m_pCardMgr=nullptr;
	CSkinMgr* m_pSkinMgr = nullptr;
	CEquipMgr* m_pEquipMgr = nullptr;
};

#endif //!_USER_H_