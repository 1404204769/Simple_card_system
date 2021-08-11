#include "LoginSystem.h"
using namespace std;
using namespace mysqlpp;
CLoginSystem::CLoginSystem() {
	stringstream strIn;
	strIn << "调用了CLoginSystem的构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CLoginSystem::~CLoginSystem() {
	stringstream strIn;
	strIn << "调用了CLoginSystem的析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CLoginSystem::Login(string& _strAccount) {
	/*参数为登入的account,调用CUserMgrSearchInMap&SearchInDB功能，若不存在返回登入失败*/
	long long int i64Id = g_UserMgr.GetOnlineUserId(_strAccount);
	if (i64Id != 0) {
		cout << "登入失败,该用户已登入！" << endl;
		return false;
	}
	mysqlpp::Row row;
	if (!g_UserMgr.CheckExistInDB(row, _strAccount)) {/*检查是否在数据库中存在，不存在则新建一个*/
		cout << "用户数据在数据库发生错误" << endl;
		return false;
	}
	if (!g_UserMgr.AddUser(row)) {
		cout << "用户加入在线队列失败" << endl;
		return false;
	}
	return true;
}

bool CLoginSystem::Logout(string& _strAccount) {
	/*删除map中的指定对象*/
	
	if (!g_UserMgr.DelUser(_strAccount)) {
		cout << "登出失败" << endl;
		return false;
	}
	return true;
}