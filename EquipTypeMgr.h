#pragma once
#ifndef _EQUIPTYPEMGR_H_
#define _EQUIPTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "EquipType.h"
class CEquipTypeMgr
{
public:
	CEquipTypeMgr();/*构造函数*/
	~CEquipTypeMgr();/*析构函数*/

	const CEquipType* Get(const unsigned int unId);/*根据装备类型获取数据*/

	void PrintAll();/*打印显示所有装备类型*/
	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/

private:
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	using EquipTypeMap = std::map<unsigned int, const CEquipType*>;
	using EquipTypeMapIter = EquipTypeMap::iterator;
	EquipTypeMap m_mapEquipType;/*每一个都代表目前已有的装备类型*/
};
extern CEquipTypeMgr g_EquipTypeMgr;
#endif //!_EQUIPTYPEMGR_H_
