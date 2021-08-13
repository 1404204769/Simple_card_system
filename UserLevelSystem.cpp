#include "UserLevelSystem.h"
using namespace std;

CUserLevelSystem::CUserLevelSystem() {
	Log("������CUserLevelSystem�Ĺ��캯��\n");
}
CUserLevelSystem::~CUserLevelSystem() {
	Log("������CUserLevelSystem����������\n");
}
bool CUserLevelSystem::LevelUp() {
	/*����ID��Ҫ���ĵȼ���ֱ����m_UserMgr��map�ڵ���set�����޸�����*/
	long long int i64Id;
	cout << "������Ҫ�������˺�ID:";
	cin >> i64Id;

	CUser* pUser = g_UserMgr.Get(i64Id);
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