#pragma once
#ifndef _USERLEVELSYSTEM_H_
#define _USERLEVELSYSTEM_H_
#include "UserMgr.h"
extern CUserMgr g_UserMgr;
class CUserLevelSystem
{
public:
	CUserLevelSystem();
	~CUserLevelSystem();
	bool LevelUp();/*传入ID和要升的等级，直接在m_UserMgr的map内调用set函数修改数据*/
private:
};


#endif //!_USERLEVELSYSTEM_H_