#pragma once
#ifndef _LOGINSYSTEM_H_
#define _LOGINSYSTEM_H_
#include "UserMgr.h"
#include <string>
extern CUserMgr g_UserMgr;
extern CDB g_DB;
class CLoginSystem
{
public:
	CLoginSystem();
	~CLoginSystem();
	bool Login();/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	bool Logout();/*ɾ��map�е�ָ������*/
};

#endif //!_LOGINSYSTEM_H_