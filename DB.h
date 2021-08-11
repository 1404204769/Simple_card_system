#pragma once
#ifndef _DB_H_
#define _DB_H_
#include <string>
#include <sstream>
#include <iostream>
#include <mysql++.h>
#include <windows.h>
#include "DebugPrint.h"
class CDB {
public:
	CDB();
	~CDB();
	bool InitConnect(const std::string& strDB, const std::string& strServer, const std::string& strUser, const std::string& strPassword, const int nPort);/*建立数据库连接*/
	mysqlpp::Query* GetQuery();/*获取一个基于当前连接的Query查询对象*/
	bool Insert(mysqlpp::Query& query);/*负责插入数据*/
	bool Delete(mysqlpp::Query& query);/*负责删除数据*/
	bool Update(mysqlpp::Query& query);/*负责更新数据*/
	bool Search(mysqlpp::UseQueryResult& res, mysqlpp::Query& query);/*负责查询数据，数据集合保存在res内*/
private:
	std::string m_strDB = "test", m_strServer = "localhost", m_strUser = "root", m_strPassword = "root";
	int m_nPort = 0;
	mysqlpp::Connection m_Conn;
};

#endif // !_DB_H_
