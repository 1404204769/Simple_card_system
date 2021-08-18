#pragma once
#ifndef _CARDRANKTYPEMGR_H_
#define _CARDRANKTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardRankType.h"
class CCardRankTypeMgr{
public:
	CCardRankTypeMgr();/*构造函数*/
	~CCardRankTypeMgr();/*析构函数*/

	const CCardRankType* Get(const unsigned int unId);/*根据卡牌阶级获取数据*/

	bool Init();/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardLevAttrTypeMgr对象的ID为0则说明数据库中无此数据*/

	bool Debug_PrintAll();/*打印显示静态配置*/

private:
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/

	using CardRankTypeMap = std::map<unsigned int, const CCardRankType*>;
	using CardRankTypeMapIter = CardRankTypeMap::iterator;
	CardRankTypeMap m_mapById;/*每一个都代表目前已有的卡牌类型*/
};
extern CCardRankTypeMgr g_CardRankTypeMgr;
#endif //!_CARDRANKTYPEMGR_H_
