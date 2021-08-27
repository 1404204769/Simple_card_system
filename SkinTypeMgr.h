#pragma once
#ifndef _SKINTYPEMGR_H_
#define _SKINTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "SkinType.h"
class CSkinTypeMgr
{
public:
	CSkinTypeMgr();/*构造函数*/
	~CSkinTypeMgr();/*析构函数*/

	const CSkinType* Get(const unsigned int unId);/*根据卡牌类型获取数据*/

	void PrintAll();/*打印显示所有卡牌类型*/
	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/

	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
private:

	using SkinTypeMap = std::map<unsigned int, const CSkinType*>;
	using SkinTypeMapIter = SkinTypeMap::iterator;
	SkinTypeMap m_mapSkinType;/*每一个都代表目前已有的卡牌类型*/
};
extern CSkinTypeMgr g_SkinTypeMgr;
#endif //!_SKINTYPEMGR_H_
