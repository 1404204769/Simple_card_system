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

	bool Login(const std::string& strAccount);/*参数为登入的account,调用CUserMgrSearchInMap&SearchInDB功能，若不存在返回登入失败*/
	bool Logout(const std::string& strAccount);/*删除map中的指定对象*/
};

#endif //!_LOGINSYSTEM_H_