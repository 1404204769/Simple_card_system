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

	bool AddCard(const std::string& strUserAccount,const unsigned int unCardType);/*Ϊָ���������һ��ָ���Ŀ���*/
	bool DelCard(const std::string& strUserAccount,const long long int i64CardId);/*Ϊָ�����ɾ��ָ������*/
	bool CardLevUp(const std::string& strUserAccount, const long long int i64CardId, const unsigned int unLev);/*Ϊָ����ҵ�ָ����������ָ���ȼ�*/
	bool ShowCard(const std::string& strUserAccount);/*��ӡ��ʾָ���û������п���*/
	bool ShowCardAtk(const std::string& strUserAccount,const long long int i64CardId);/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/

	bool Wear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*ָ���û�ָ�����ƴ�ָ��Ƥ��*/
	bool Drop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*ָ���û�ָ��������ָ��Ƥ��*/
	bool AddSkin(const long long int i64UserId,const unsigned int unSkinType);/*ָ���û����ָ������ָ��Ƥ��*/
	bool ShowSkin(const long long int i64UserId);/*չʾ�û�ӵ�е�����Ƥ��*/

	bool CardRankLevUp(const long long int i64UserId, const long long int i64CardId, std::vector<long long int>&vecConsume);/*Ϊָ����ҵ�ָ�����������׼�*/
	bool IsLowValue(CCard* const pCard) const;/*�жϿ����Ƿ��ǵͼ�ֵ��*/
};


#endif //!_CARDSYSTEM_H_

