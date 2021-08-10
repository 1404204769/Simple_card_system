#pragma once
#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_
#include <string>
#include <sstream>
#include "DebugPrint.h"
#include "UserMgr.h"
#include "CardTypeMgr.h"
#include "CardMgr.h"
extern CUserMgr g_UserMgr;
extern CCardTypeMgr g_CardTypeMgr;
class CCardSystem
{
public:
	CCardSystem();
	~CCardSystem();
	bool AddUserCard(std::string& _strUserAccount, unsigned int _unCardType);/*Ϊָ���������һ��ָ���Ŀ���*/
	bool DelUserCard(std::string& _strUserAccount, std::string& _strCardName);/*Ϊָ�����ɾ��ָ������*/
	bool ShowUserCard(std::string& _strUserAccount);/*��ӡ��ʾָ���û������п���*/
	bool ShowUserCardAtk(std::string& _strUserAccount, std::string& _strCardName);/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
};


#endif //!_CARDSYSTEM_H_

