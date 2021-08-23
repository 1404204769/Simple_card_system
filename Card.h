#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "CardType.h"
#include "SkinMgr.h"
#include "CardRankTypeMgr.h"
#include "CardLevAttrTypeMgr.h"
#include "EquipTypeMgr.h"
class CCard
{
public:
	CCard();/*���캯��*/
	~CCard();/*��������*/

	bool CreateNewCard(const long long int i64UserId,const CCardType* pCardType);/*����һ���µĿ���*/
	bool CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType);/*�����ݿ��м��ؿ���*/

	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetLev() const;/*��ȡm_unLev*/
	unsigned int GetCardRankLev() const;/*��ȡm_unRankLev*/
	unsigned int GetCardType() const;/*��ȡm_unCardType*/
	long long int GetCardId() const;/*��ȡm_i64CardId*/
	long long int GetUserId() const;/*��ȡm_i64UserId*/
	long long int GetExp() const;/*��ȡm_i64Exp*/
	long long int GetPosHat()const;/*��ȡm_i64EquipPosHat*/
	long long int GetPosArmour()const;/*��ȡm_i64PosArmour*/
	long long int GetPosShoes()const;/*��ȡm_i64PosShoes*/
	const CCardType& GetCardTypeData() const;/*��ȡ��Ӧ�Ŀ���������ϸ����*/
	const CCardLevAttrType& GetCardLevAttrTypeData() const;/*��ȡ��Ӧ���Ƶȼ��ļӳ�����*/
	const CCardRankType& GetCardRankTypeData() const;/*��ȡ��Ӧ���ƽ׼��ļӳ�����*/
	
	bool LevelUp(unsigned int unLev);/*�������Ƶȼ��������ӳ�����*/
	bool RankUp();/*�������ƽ׼��������ӳ�����*/

	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/

private:
	bool Update();/*�������ݿ�*/
	bool Insert();/*��Card���ݲ������ݿ�*/

	bool SetLevel(unsigned int unLev);/*����Level�ȼ�*/
	bool SetRank(unsigned int unRank);/*����Rank�ȼ�*/
	bool SetPosHat(long long int i64EquipId);/*����ͷ��װ����Ӧװ��ID*/
	bool SetPosArmour(long long int i64EquipId);/*�����¼�װ����Ӧװ��ID*/
	bool SetPosShoes(long long int i64EquipId);/*����Ь��װ����Ӧװ��ID*/

	long long int m_i64EquipPosHat = 0, m_i64EquipPosArmour = 0, m_i64EquipPosShoes = 0;
	unsigned int  m_unCardType = 0, m_unLev = 0,m_unRankLev=0;
	long long int m_i64CardId = 0, m_i64UserId = 0, m_i64Exp = 0;
	std::string m_strName;
	const CCardType* m_pCardType = nullptr;
	const CCardLevAttrType* m_pCardLevAttrType = nullptr;
	const CCardRankType* m_pCardRankType = nullptr;
};
#endif //!_CARD_H_
