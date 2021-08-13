#pragma once
#ifndef _USERMGR_H_
#define _USERMGR_H_
#include <map>
#include "DB.h"
#include "User.h"
#include <windows.h>
#include <mysql++.h>
class CUserMgr {
public://�ڲ����߼�����
	CUserMgr();
	~CUserMgr();

	/*
	* ������ҵ�����ؽӿ�
	*/
	CUser* Get(const long long int i64Id);/*��map�����ѯ�����ز�ѯ����ָ��*/
	long long int GetOnlineId(const std::string& strAccount);/*�����˻�����ID*//*�������һ������˵�����û�������*/

	bool Add(const std::string& stdAccount);/*�������Map����������*/
	bool Del(const std::string& strAccount);/*�������Map������ɾ��*/
	void PrintOnline();/*��ӡ��ʾ����������ҵ���Ϣ*/

	bool CheckExistInDB(mysqlpp::Row& row, const std::string& strAccount);/*����û��Ƿ���������ݿ�*/
private:
	/*
	* �����Ƕ�˽�������Ĳ��������ܱ�¶�����
	*/
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	
	bool SearchDB(mysqlpp::Row& row, const std::string& strAccount);/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/
	bool Generate(const std::string& strAccount);/*ע�����û�������Ϊaccount��name,����CUser��InsertUser*/
	
	using UserIdMap = std::map<long long int, CUser*>;
	using UserIdMapIter = UserIdMap::iterator;
	UserIdMap m_mapById;/*ÿһ��������Ŀǰ����������û�*/

	using UserAccountMap = std::map<std::string, long long int>;
	using UserAccountMapIter = UserAccountMap::iterator;
	UserAccountMap m_mapByAccount;/*��¼Account���û�ID֮��Ĺ�ϵ*/
};
extern CUserMgr g_UserMgr;
#endif // !_USERMGR_H_

