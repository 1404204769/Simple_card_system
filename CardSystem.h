#pragma once
#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_
#include <string>
#include <sstream>
#include "CardMgr.h"
#include "UserMgr.h"
#include "DebugPrint.h"
#include "CardTypeMgr.h"
extern CUserMgr g_UserMgr;
extern CCardTypeMgr g_CardTypeMgr;
class CCardSystem
{
public:
	CCardSystem();
	~CCardSystem();
	bool AddUserCard(std::string& _strUserAccount, unsigned int _unCardType);/*为指定玩家增加一副指定的卡牌*/
	bool DelUserCard(std::string& _strUserAccount, long long int _i64CardId);/*为指定玩家删除指定卡牌*/
	bool ShowUserCard(std::string& _strUserAccount);/*打印显示指定用户的所有卡牌*/
	bool ShowUserCardAtk(std::string& _strUserAccount, long long int _i64CardId);/*打印显示指定用户的指定卡牌的攻击力*/
};


#endif //!_CARDSYSTEM_H_

