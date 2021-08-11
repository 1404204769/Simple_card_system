#include "UserLevelSystem.h"
using namespace std;

CUserLevelSystem::CUserLevelSystem() {
	stringstream strIn;
	strIn << "������CUserLevelSystem�Ĺ��캯��\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CUserLevelSystem::~CUserLevelSystem() {
	stringstream strIn;
	strIn << "������CUserLevelSystem����������\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CUserLevelSystem::LevelUp() {
	/*����ID��Ҫ���ĵȼ���ֱ����m_UserMgr��map�ڵ���set�����޸�����*/
	long long int i64Id;
	cout << "������Ҫ�������˺�ID:";
	cin >> i64Id;
	CUser* pUser = g_UserMgr.GetUser(i64Id);
	if (!pUser) {
		cout << "���˻������ڣ������²���" << endl;
		return false;
	}
	cout << "������Ҫ���ĵȼ�:";
	unsigned int unLev;
	cin >> unLev;
	pUser->SetLev(pUser->GetLev() + unLev);
	return true;
}