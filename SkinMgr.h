#pragma once
#ifndef _SKINMGR_H_
#define _SKINMGR_H_
#include "DB.h"
#include "Skin.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "SkinTypeMgr.h"
class CUser;
class CCardMgr;
class CSkinMgr
{
public:
	CSkinMgr();/*构造函数*/
	~CSkinMgr();/*析构函数*/

	bool Init(CUser* pUser);/*函数内部连接数据库获取数据*/

	CSkin* Get(const long long int i64SkinId);/*根据皮肤ID获取皮肤数据*/
	CSkin* GetWearing(const long long int i64CardId);/*根据皮肤ID获取皮肤数据*/

	bool AddNew(const CSkinType* pSkinType);/*增加一张指定类型的皮肤*/

	void DeBug_DelAll();/*删除所有的皮肤*/
	void PrintAll();/*打印显示拥有的所有皮肤牌的数据*/
	bool Wear(const long long int i64CardId, const long long int i64SkinId);/*指定某张卡牌穿上某个皮肤*/
	bool Drop(const long long int i64CardId, const long long int i64SkinId);/*指定某张卡牌脱下某个皮肤*/

private:
	bool Add(CSkin* pSkin);/*容器增加操作，choice=1：增加已有皮肤数据,choice=2:增加新皮肤*/
	bool Del(long long int i64SkinId);/*根据玩家皮肤ID来删除数据*/
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	CUser* m_pUser = nullptr;
	using SkinTypeSet = std::set<unsigned int>;
	SkinTypeSet m_setBySkinType;

	using CardSkinMap = std::map<long long int, CSkin*>;
	using CardSkinMapIter = CardSkinMap::iterator;
	CardSkinMap m_mapByCardId;/*每一个都代表目前被卡牌穿戴的皮肤*/

	using SkinMap = std::map<long long int, CSkin*>;
	using SkinMapIter = SkinMap::iterator;
	SkinMap m_mapBySkinId;/*每一个都代表目前已有的卡牌类型*/
};
#endif //!_SKINMGR_H_
