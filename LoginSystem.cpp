#include "LoginSystem.h"
using namespace std;
using namespace mysqlpp;
CLoginSystem::CLoginSystem() {
	Log("������CLoginSystem�Ĺ��캯��\n");
}
CLoginSystem::~CLoginSystem() {
	Log("������CLoginSystem����������\n");
}
bool CLoginSystem::Login(const string& _strAccount) {
	/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	if (!g_UserMgr.Add(_strAccount)) {
		cout << "����ʧ��" << endl;
		return false;
	}
	return true;
}

bool CLoginSystem::Logout(const string& _strAccount) {
	/*ɾ��map�е�ָ������*/
	if (!g_UserMgr.Del(_strAccount)) {
		cout << "�ǳ�ʧ��" << endl;
		return false;
	}
	return true;
}