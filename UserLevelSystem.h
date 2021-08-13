#pragma once
#ifndef _USERLEVELSYSTEM_H_
#define _USERLEVELSYSTEM_H_
#include "UserMgr.h"
class CUserLevelSystem
{
public:
	CUserLevelSystem();
	~CUserLevelSystem();

	bool LevelUp();/*传入ID和要升的等级，直接在m_UserMgr的map内调用set函数修改数据*/
};


#endif //!_USERLEVELSYSTEM_H_