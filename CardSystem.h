#pragma once
#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_
#include <string>
#include "CardMgr.h"
#include "EquipMgr.h"
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

	bool SkinWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*ָ���û�ָ�����ƴ�ָ��Ƥ��*/
	bool SkinDrop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId);/*ָ���û�ָ��������ָ��Ƥ��*/
	bool SkinAdd(const long long int i64UserId,const unsigned int unSkinType);/*ָ���û����ָ������ָ��Ƥ��*/
	bool SkinShow(const long long int i64UserId);/*չʾ�û�ӵ�е�����Ƥ��*/

	bool CardRankLevUp(const long long int i64UserId, const long long int i64CardId, std::vector<long long int>&vecConsume);/*Ϊָ����ҵ�ָ�����������׼�*/
	bool IsLowValue(CCard* const pCard) const;/*�жϿ����Ƿ��ǵͼ�ֵ��*/

	bool EquipWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64EquipId);/*ָ���û�ָ�����ƴ�ָ��װ��*/
	bool EquipDrop(const long long int i64UserId, const long long int  i64EquipId);/*ָ���û���ָ��װ��*/
	bool EquipAdd(const long long int i64UserId, const unsigned int unEquipType);/*ָ���û����ָ������װ��*/
	bool EquipShow(const long long int i64UserId);/*չʾ�û�ӵ�е�����װ��*/

};


#endif //!_CARDSYSTEM_H_

