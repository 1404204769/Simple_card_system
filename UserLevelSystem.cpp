#include "UserLevelSystem.h"
using namespace std;

CUserLevelSystem::CUserLevelSystem() {
	cout << "������CUserLevelSystem�Ĺ��캯��" << endl;
}
CUserLevelSystem::~CUserLevelSystem() {
	cout << "������CUserLevelSystem����������" << endl;
}
bool CUserLevelSystem::LevelUp() {
	/*����ID��Ҫ���ĵȼ���ֱ����m_UserMgr��map�ڵ���set�����޸�����*/
	long long int i64Id;
	cout << "������Ҫ�������˺�ID:";
	cin >> i64Id;
	CUser* pUser = g_UserMgr.getUser(i64Id);
	if (!pUser) {
		cout << "���˻������ڣ������²���" << endl;
		return false;
	}
	cout << "������Ҫ���ĵȼ�:";
	unsigned int unLev;
	cin >> unLev;
	pUser->setLev(pUser->getLev() + unLev);
	return true;
}