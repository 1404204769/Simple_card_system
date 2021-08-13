#pragma once
#ifndef _USER_H_
#define _USER_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
class CCardMgr;
class CUser
{
public:
	CUser(const std::string& strAccount);
	~CUser();/*当用户退出时调用此析构函数，并调用CDB::UpdateData函数将数据保存在数据库中*/

	const std::string& GetAccount() const;
	const std::string& GetCreateTime() const;
	const std::string& GetName() const;
	long long int GetId() const;
	long long int GetExp() const;
	unsigned int GetLev() const;
	CCardMgr& GetCardMgr() const;
	void SetLev(const unsigned int unLev);
	void SetExp(const long long int i64Exp);
	void SetName(const std::string& strName);

	/*
	* 以下是数据库层相关接口
	*/

	bool Init(const mysqlpp::Row& row);/*根据查询获取的mysqlpp::Row对User对象初始化*/
	bool Insert();/*在数据库中插入新的User数据*/
	bool Delete();/*在数据库中删除指定User数据*/

private:
	bool Update(CUser& pUser);/*在数据库中更新指定User*/

	unsigned int m_unLev = 0;
	long long int m_i64Id = 0, m_i64Exp = 0;
	std::string m_strAccount, m_strCreateTime, m_strName;
	CCardMgr* m_pCardMgr=nullptr;
};

#endif //!_USER_H_