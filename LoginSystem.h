#pragma once
#ifndef _LOGINSYSTEM_H_
#define _LOGINSYSTEM_H_
#include <string>
#include "UserMgr.h"
class CLoginSystem
{
public:
	CLoginSystem();
	~CLoginSystem();

	bool Login(const std::string& strAccount);/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	bool Logout(const std::string& strAccount);/*ɾ��map�е�ָ������*/
};

#endif //!_LOGINSYSTEM_H_