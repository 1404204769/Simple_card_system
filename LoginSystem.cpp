#include "LoginSystem.h"
using namespace std;
using namespace mysqlpp;
CLoginSystem::CLoginSystem() {
	Log("调用了CLoginSystem的构造函数\n");
}
CLoginSystem::~CLoginSystem() {
	Log("调用了CLoginSystem的析构函数\n");
}
bool CLoginSystem::Login(const string& _strAccount) {
	/*参数为登入的account,调用CUserMgrSearchInMap&SearchInDB功能，若不存在返回登入失败*/
	if (!g_UserMgr.Add(_strAccount)) {
		cout << "登入失败" << endl;
		return false;
	}
	return true;
}

bool CLoginSystem::Logout(const string& _strAccount) {
	/*删除map中的指定对象*/
	if (!g_UserMgr.Del(_strAccount)) {
		cout << "登出失败" << endl;
		return false;
	}
	return true;
}