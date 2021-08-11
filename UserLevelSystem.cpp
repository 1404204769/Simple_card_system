#include "UserLevelSystem.h"
using namespace std;

CUserLevelSystem::CUserLevelSystem() {
	stringstream strIn;
	strIn << "调用了CUserLevelSystem的构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CUserLevelSystem::~CUserLevelSystem() {
	stringstream strIn;
	strIn << "调用了CUserLevelSystem的析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CUserLevelSystem::LevelUp() {
	/*传入ID和要升的等级，直接在m_UserMgr的map内调用set函数修改数据*/
	long long int i64Id;
	cout << "请输入要升级的账号ID:";
	cin >> i64Id;
	CUser* pUser = g_UserMgr.GetUser(i64Id);
	if (!pUser) {
		cout << "此账户不存在，请重新操作" << endl;
		return false;
	}
	cout << "请输入要升的等级:";
	unsigned int unLev;
	cin >> unLev;
	pUser->SetLev(pUser->GetLev() + unLev);
	return true;
}