#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include "DB.h"
#include <map>
#include <string>
#include <assert.h>
#include "SkinMgr.h"
#include <mysql++.h>
#include "CardType.h"
#include "EquipTypeMgr.h"
#include "CardRankTypeMgr.h"
#include "CardLevAttrTypeMgr.h"
class CCard
{
public:
	static const unsigned int EquipPosMax = 3;
	using EquipPosMap = std::map<unsigned int, long long int>;//key Ϊ װ��λPos  valueΪװ��Id
	using EquipIter = EquipPosMap::const_iterator;

public:
	CCard();/*���캯��*/
	~CCard();/*��������*/

	bool CreateNewCard(const long long int i64UserId,const CCardType* pCardType);/*����һ���µĿ���*/
	bool CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType);/*�����ݿ��м��ؿ���*/


	void GetFieldNamePos(std::string& strFieldName_Out, const unsigned int unPos);/*��ȡ��Ӧ���ݿ��е�����λ�ֶ���*/
	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetLev() const;/*��ȡm_unLev*/
	unsigned int GetCardRankLev() const;/*��ȡm_unRankLev*/
	unsigned int GetCardType() const;/*��ȡm_unCardType*/
	long long int GetCardId() const;/*��ȡm_i64CardId*/
	long long int GetUserId() const;/*��ȡm_i64UserId*/
	long long int GetExp() const;/*��ȡm_i64Exp*/
	long long int GetEquipPos(const unsigned int unPos);/*��ȡָ��λ���ϵ�װ��*/
	const CCardType& GetCardTypeData() const;/*��ȡ��Ӧ�Ŀ���������ϸ����*/
	const CCardLevAttrType& GetCardLevAttrTypeData() const;/*��ȡ��Ӧ���Ƶȼ��ļӳ�����*/
	const CCardRankType& GetCardRankTypeData() const;/*��ȡ��Ӧ���ƽ׼��ļӳ�����*/
	
	EquipIter GetEquipIterBegin();/*���װ�����������*/
	EquipIter GetEquipIterEnd();/*���װ���������յ�*/

	bool LevelUp(unsigned int unLev);/*�������Ƶȼ��������ӳ�����*/
	bool RankUp();/*�������ƽ׼��������ӳ�����*/

	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/

	bool Wear(const long long int i64EquipId, const CEquipType* pEquipType);/*����װ��*/
	bool Drop(const unsigned int unPos);/*����װ��*/
	
private:
	bool Update();/*�������ݿ�*/
	bool Insert();/*��Card���ݲ������ݿ�*/
	bool SetEquip(const long long int i64EquipId, const unsigned int unPos);/*����װ��λ*/
	bool SetLevel(unsigned int unLev);/*����Level�ȼ�*/
	bool SetRank(unsigned int unRank);/*����Rank�ȼ�*/

private:
	EquipPosMap m_mapEquipPos;

	unsigned int  m_unCardType = 0, m_unLev = 0,m_unRankLev=0;
	long long int m_i64CardId = 0, m_i64UserId = 0, m_i64Exp = 0;
	std::string m_strName;
	const CCardType* m_pCardType = nullptr;
	const CCardLevAttrType* m_pCardLevAttrType = nullptr;
	const CCardRankType* m_pCardRankType = nullptr;
};
#endif //!_CARD_H_
