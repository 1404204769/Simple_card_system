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
	bool LevelUp();/*����ID��Ҫ���ĵȼ���ֱ����m_UserMgr��map�ڵ���set�����޸�����*/
private:
};


#endif //!_USERLEVELSYSTEM_H_