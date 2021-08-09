#include "UserLevelSystem.h"
using namespace std;

CUserLevelSystem::CUserLevelSystem() {
	cout << "调用了CUserLevelSystem的构造函数" << endl;
}
CUserLevelSystem::~CUserLevelSystem() {
	cout << "调用了CUserLevelSystem的析构函数" << endl;
}
bool CUserLevelSystem::LevelUp() {
	/*传入ID和要升的等级，直接在m_UserMgr的map内调用set函数修改数据*/
	long long int i64Id;
	cout << "请输入要升级的账号ID:";
	cin >> i64Id;
	CUser* pUser = g_UserMgr.getUser(i64Id);
	if (!pUser) {
		cout << "此账户不存在，请重新操作" << endl;
		return false;
	}
	cout << "请输入要升的等级:";
	unsigned int unLev;
	cin >> unLev;
	pUser->setLev(pUser->getLev() + unLev);
	return true;
}