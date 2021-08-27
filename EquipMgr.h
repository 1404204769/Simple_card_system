#pragma once
#ifndef _EQUIPMGR_H_
#define _EQUIPMGR_H_
#include "DB.h"
#include "Equip.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "EquipTypeMgr.h"
class CUser;
class CCardMgr;
class CEquipMgr
{
public:
	CEquipMgr();/*构造函数*/
	~CEquipMgr();/*析构函数*/

	bool Init(CUser* pUser);/*函数内部连接数据库获取数据*/
	bool IsInit();/*返回是否初始化*/

	CEquip* Get(const long long int i64EquipId);/*根据装备ID获取皮肤数据*/

	bool AddNew(const CEquipType* pEquipType);/*增加一张指定类型的装备*/
	bool DelExist(const long long int i64EquipId);/*删除一张已有的装备*/
	void PrintAll();/*打印显示拥有的所有装备的数据*/
	long long int GetWearCardId(const long long int i64EquipId);/*查询装备被哪张卡牌穿了*/

	bool AddEquipCardMap(const long long int i64EquipId, const long long int i64CardId);/*穿戴标记容器增加操作*/
	bool DelEquipCardMap(const long long int i64EquipId);/*穿戴标记容器删除操作*/
private:
	bool Add(CEquip* pEquip);/*容器增加操作*/
	bool Del(const long long int i64EquipId);/*根据装备ID来删除数据*/
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	CUser* m_pUser = nullptr;
	bool m_bInit = false;

	using EquipCardMap = std::map<long long int,long long int>;//EquipID作为key CardId作为value
	using EquipCardMapIter = EquipCardMap::iterator;
	EquipCardMap m_mapEquipCard;/*每一个都代表目前被卡牌穿戴的装备*/

	using EquipMap = std::map<long long int, CEquip*>;
	using EquipMapIter = EquipMap::iterator;
	EquipMap m_map;/*每一个都代表目前已有的装备*/
};
#endif //!_EQUIPMGR_H_
