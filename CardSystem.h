#pragma once
#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_
#include <string>
#include "CardMgr.h"
#include "UserMgr.h"
#include "DebugPrint.h"
#include "CardTypeMgr.h"
class CCardSystem
{
public:
	CCardSystem();
	~CCardSystem();

	bool Add(const std::string& strUserAccount,const unsigned int unCardType);/*为指定玩家增加一副指定的卡牌*/
	bool Del(const std::string& strUserAccount,const long long int i64CardId);/*为指定玩家删除指定卡牌*/
	bool Show(const std::string& strUserAccount);/*打印显示指定用户的所有卡牌*/
	bool ShowAtk(const std::string& strUserAccount,const long long int i64CardId);/*打印显示指定用户的指定卡牌的攻击力*/
};


#endif //!_CARDSYSTEM_H_

