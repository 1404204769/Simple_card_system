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

	CSkin* Get(const long long int i64SkinId);/*根据卡牌名称获取卡牌数据*/

	bool Add(const long long int i64UserId, const CSkinType* pSkinType);/*增加一张指定类型的卡牌*/
	bool Del(long long int i64SkinId);/*根据玩家卡牌名称来删除数据*/
	bool DelAll();/*删除所有的皮肤*/
	void PrintAll() const;/*打印显示拥有的所有皮肤牌的数据*/
	bool Wear(const long long int i64CardId, const long long int i64SkinId);/*指定某张卡牌穿上某个皮肤*/
	bool Drop(const long long int i64CardId, const long long int i64SkinId);/*指定某张卡牌脱下某个皮肤*/

private:

	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	CUser* m_pUser = nullptr;

	using SkinMap = std::map<long long int, CSkin*>;
	using SkinMapIter = SkinMap::iterator;
	SkinMap m_mapBySkinId;/*每一个都代表目前已有的卡牌类型*/
};
#endif //!_SKINMGR_H_
