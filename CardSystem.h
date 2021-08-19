#pragma once
#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_
#include <string>
#include "CardMgr.h"
#include "UserMgr.h"
#include "DebugPrint.h"
#include "CardTypeMgr.h"
#include "CardRankTypeMgr.h"
class CCardSystem
{
public:
	CCardSystem();
	~CCardSystem();

	bool AddCard(const std::string& strUserAccount,const unsigned int unCardType);/*为指定玩家增加一副指定的卡牌*/
	bool DelCard(const std::string& strUserAccount,const long long int i64CardId);/*为指定玩家删除指定卡牌*/
	bool CardLevUp(const std::string& strUserAccount, const long long int i64CardId, const unsigned int unLev);/*为指定玩家的指定卡牌提升指定等级*/
	bool ShowCard(const std::string& strUserAccount);/*打印显示指定用户的所有卡牌*/
	bool ShowCardAtk(const std::string& strUserAccount,const long long int i64CardId);/*打印显示指定用户的指定卡牌的攻击力*/

	bool Wear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*指定用户指定卡牌穿指定皮肤*/
	bool Drop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*指定用户指定卡牌脱指定皮肤*/
	bool AddSkin(const long long int i64UserId,const unsigned int unSkinType);/*指定用户获得指定类型指定皮肤*/
	bool ShowSkin(const long long int i64UserId);/*展示用户拥有的所有皮肤*/

	bool CardRankLevUp(const long long int i64UserId, const long long int i64CardId, std::vector<long long int>&vecConsume);/*为指定玩家的指定卡牌提升阶级*/
	bool IsLowValue(CCard* const pCard) const;/*判断卡牌是否是低价值的*/
};


#endif //!_CARDSYSTEM_H_

