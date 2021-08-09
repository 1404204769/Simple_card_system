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
	bool Login();/*参数为登入的account,调用CUserMgrSearchInMap&SearchInDB功能，若不存在返回登入失败*/
	bool Logout();/*删除map中的指定对象*/
};

#endif //!_LOGINSYSTEM_H_