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
	bool InitConnect(const std::string& strDB, const std::string& strServer, const std::string& strUser, const std::string& strPassword, const int nPort);/*�������ݿ�����*/
	mysqlpp::Query* GetQuery();/*��ȡһ�����ڵ�ǰ���ӵ�Query��ѯ����*/
	bool Insert(mysqlpp::Query& query);/*�����������*/
	bool Delete(mysqlpp::Query& query);/*����ɾ������*/
	bool Update(mysqlpp::Query& query);/*�����������*/
	bool Search(mysqlpp::UseQueryResult& res, mysqlpp::Query& query);/*�����ѯ���ݣ����ݼ��ϱ�����res��*/
private:
	std::string m_strDB = "test", m_strServer = "localhost", m_strUser = "root", m_strPassword = "root";
	int m_nPort = 0;
	mysqlpp::Connection m_Conn;
};

#endif // !_DB_H_
