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

	bool Add(const std::string& strUserAccount,const unsigned int unCardType);/*Ϊָ���������һ��ָ���Ŀ���*/
	bool Del(const std::string& strUserAccount,const long long int i64CardId);/*Ϊָ�����ɾ��ָ������*/
	bool Show(const std::string& strUserAccount);/*��ӡ��ʾָ���û������п���*/
	bool ShowAtk(const std::string& strUserAccount,const long long int i64CardId);/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
};


#endif //!_CARDSYSTEM_H_

