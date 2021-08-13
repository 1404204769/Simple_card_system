#pragma once
#ifndef _USERMGR_H_
#define _USERMGR_H_
#include <map>
#include "DB.h"
#include "User.h"
#include <windows.h>
#include <mysql++.h>
class CUserMgr {
public://内部的逻辑函数
	CUserMgr();
	~CUserMgr();

	/*
	* 以下是业务层相关接口
	*/
	CUser* Get(const long long int i64Id);/*从map里面查询，返回查询对象指针*/
	long long int GetOnlineId(const std::string& strAccount);/*根据账户返回ID*//*如果返回一个负数说明该用户不在线*/

	bool Add(const std::string& stdAccount);/*负责管理Map容器的增加*/
	bool Del(const std::string& strAccount);/*负责管理Map容器的删除*/
	void PrintOnline();/*打印显示所有在线玩家的信息*/

	bool CheckExistInDB(mysqlpp::Row& row, const std::string& strAccount);/*检查用户是否存在于数据库*/
private:
	/*
	* 以下是对私有容器的操作，不能暴露给外层
	*/
	void Free();/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	
	bool SearchDB(mysqlpp::Row& row, const std::string& strAccount);/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/
	bool Generate(const std::string& strAccount);/*注册新用户，参数为account，name,调用CUser的InsertUser*/
	
	using UserIdMap = std::map<long long int, CUser*>;
	using UserIdMapIter = UserIdMap::iterator;
	UserIdMap m_mapById;/*每一个都代表目前登入的在线用户*/

	using UserAccountMap = std::map<std::string, long long int>;
	using UserAccountMapIter = UserAccountMap::iterator;
	UserAccountMap m_mapByAccount;/*记录Account与用户ID之间的关系*/
};
extern CUserMgr g_UserMgr;
#endif // !_USERMGR_H_

