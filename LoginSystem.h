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
	bool Login(std::string& _strAccount);/*参数为登入的account,调用CUserMgrSearchInMap&SearchInDB功能，若不存在返回登入失败*/
	bool Logout(std::string& _strAccount);/*删除map中的指定对象*/
};

#endif //!_LOGINSYSTEM_H_