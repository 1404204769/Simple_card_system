#pragma once
#ifndef _CARDLEVATTRTYPEMGR_H_
#define _CARDLEVATTRTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardLevAttrType.h"
class CCardLevAttrTypeMgr
{
public:
	CCardLevAttrTypeMgr();/*构造函数*/
	~CCardLevAttrTypeMgr();/*析构函数*/

	const CCardLevAttrType* Get(const unsigned int unLev);/*根据卡牌Lev获取数据*/

	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardLevAttrTypeMgr对象的ID为0则说明数据库中无此数据*/

private:
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	using CardLevAttrTypeMap = std::map<unsigned int, const CCardLevAttrType*>;
	using CardLevAttrTypeMapIter = CardLevAttrTypeMap::iterator;
	CardLevAttrTypeMap m_mapByLev;/*每一个都代表目前已有的卡牌类型*/
};
extern CCardLevAttrTypeMgr g_CardLevAttrTypeMgr;
#endif //!_CARDLEVATTRTYPEMGR_H_
