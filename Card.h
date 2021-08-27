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
	using EquipPosMap = std::map<unsigned int, long long int>;//key 为 装备位Pos  value为装备Id
	using EquipIter = EquipPosMap::const_iterator;

public:
	CCard();/*构造函数*/
	~CCard();/*析构函数*/

	bool CreateNewCard(const long long int i64UserId,const CCardType* pCardType);/*构造一个新的卡牌*/
	bool CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType);/*从数据库中加载卡牌*/


	void GetFieldNamePos(std::string& strFieldName_Out, const unsigned int unPos);/*获取对应数据库中的数据位字段名*/
	const std::string& GetName() const;/*获取m_strName*/
	unsigned int GetLev() const;/*获取m_unLev*/
	unsigned int GetCardRankLev() const;/*获取m_unRankLev*/
	unsigned int GetCardType() const;/*获取m_unCardType*/
	long long int GetCardId() const;/*获取m_i64CardId*/
	long long int GetUserId() const;/*获取m_i64UserId*/
	long long int GetExp() const;/*获取m_i64Exp*/
	long long int GetEquipPos(const unsigned int unPos);/*获取指定位置上的装备*/
	const CCardType& GetCardTypeData() const;/*获取对应的卡牌类型详细数据*/
	const CCardLevAttrType& GetCardLevAttrTypeData() const;/*获取对应卡牌等级的加成数据*/
	const CCardRankType& GetCardRankTypeData() const;/*获取对应卡牌阶级的加成数据*/
	
	EquipIter GetEquipIterBegin();/*获得装备迭代器起点*/
	EquipIter GetEquipIterEnd();/*获得装备迭代器终点*/

	bool LevelUp(unsigned int unLev);/*提升卡牌等级，更换加成数据*/
	bool RankUp();/*提升卡牌阶级，更换加成数据*/

	bool Delete();/*将Card数据从数据库删除*/

	bool Wear(const long long int i64EquipId, const CEquipType* pEquipType);/*穿戴装备*/
	bool Drop(const unsigned int unPos);/*脱下装备*/
	
private:
	bool Update();/*更新数据库*/
	bool Insert();/*将Card数据插入数据库*/
	bool SetEquip(const long long int i64EquipId, const unsigned int unPos);/*设置装备位*/
	bool SetLevel(unsigned int unLev);/*设置Level等级*/
	bool SetRank(unsigned int unRank);/*设置Rank等级*/

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
