#pragma once
#ifndef _LOGINSYSTEM_H_
#define _LOGINSYSTEM_H_
#include "UserMgr.h"
#include <string>
extern CDB g_DB;
extern CUserMgr g_UserMgr;
class CLoginSystem
{
public:
	CLoginSystem();
	~CLoginSystem();
	bool Login(std::string& _strAccount);/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	bool Logout(std::string& _strAccount);/*ɾ��map�е�ָ������*/
};

#endif //!_LOGINSYSTEM_H_