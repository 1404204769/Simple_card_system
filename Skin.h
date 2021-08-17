#pragma once
#ifndef _SKIN_H_
#define _SKIN_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "SkinType.h"
class CCard;
class CSkin
{
public:
	CSkin();/*构造函数*/
	~CSkin();/*析构函数*/

	bool CreateNewSkin(const long long int i64UserId, const CSkinType* pSkinType);/*构造一个新的卡牌*/
	bool CreateFromDB(const mysqlpp::Row& row, const CSkinType* pSkinType);/*从数据库中加载卡牌*/

	unsigned int GetSkinType() const;/*获取m_unSkinType*/
	long long int GetCardId() const;/*获取m_i64CardId*/
	long long int GetSkinId() const;/*获取m_i64SkinId*/
	long long int GetUserId() const;/*获取m_i64UserId*/

	bool IsWear()const;/*查看该皮肤是否被穿戴*/
	bool Wear(CCard& Card);/*穿上某个皮肤*/
	bool Drop(CCard& Card);/*脱下某个皮肤*/


	bool Delete();/*将Card数据从数据库删除*/
private:
	bool Insert(long long int& i64SkinId_Out);/*将Card数据插入数据库*/
	bool Update();/*将最新的数据更新到数据库*/
	unsigned int  m_unSkinType = 0;
	long long int m_i64SkinId = 0, m_i64UserId = 0,m_i64CardId=0;
};
#endif //!_SKIN_H_
