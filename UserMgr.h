#pragma once
#ifndef _USERMGR_H_
#define _USERMGR_H_
#include <map>
#include "DB.h"
#include "User.h"
#include <windows.h>
#include <mysql++.h>
extern CDB g_DB;
class CUserMgr {
public://�ڲ����߼�����
	CUserMgr();
	~CUserMgr();
	/*
	* ������ҵ�����ؽӿ�
	*/
	bool AddUser(mysqlpp::Row& row);/*�������Map����������*/
	bool DelUser(const std::string& _strAccount);/*�������Map������ɾ��*/
	CUser* getUser(const long long int _i64Id);/*��map�����ѯ�����ز�ѯ����ָ��*/
	long long int getOnlineUserId(const std::string& _strAccount);/*�����˻�����ID*//*�������һ������˵�����û�������*/
	void PrintOnlineUser();/*��ӡ��ʾ����������ҵ���Ϣ*/
	bool CheckExistInDB(mysqlpp::Row& row, const std::string& _strAccount);/*����û��Ƿ���������ݿ�*/
private:
	/*
	* �����Ƕ�˽�������Ĳ��������ܱ�¶�����
	*/
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	bool SearchUser(mysqlpp::Row& row, const std::string& _strAccount);/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/
	bool GenerateUser(const std::string& _strAccount);/*ע�����û�������Ϊaccount��name,����CUser��InsertUser*/
	using MapById = std::map<long long int, CUser*>;
	using MapByIdIter = MapById::iterator;
	MapById m_mapById;/*ÿһ��������Ŀǰ����������û�*/
	using MapByAccount = std::map<std::string, long long int>;
	using MapByAccountIter = MapByAccount::iterator;
	MapByAccount m_mapByAccount;/*��¼Account���û�ID֮��Ĺ�ϵ*/
};
#endif // !_USERMGR_H_

