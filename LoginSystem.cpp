#include "LoginSystem.h"
using namespace std;
using namespace mysqlpp;
CLoginSystem::CLoginSystem() {
	cout << "������CLoginSystem�Ĺ��캯��" << endl;
}
CLoginSystem::~CLoginSystem() {
	cout << "������CLoginSystem����������" << endl;
}
bool CLoginSystem::Login(string& _strAccount) {
	/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	
	long long int i64Id = g_UserMgr.getOnlineUserId(_strAccount);
	if (i64Id != 0) {
		cout << "����ʧ��,���û��ѵ��룡" << endl;
		return false;
	}
	mysqlpp::Row row;
	if (!g_UserMgr.CheckExistInDB(row, _strAccount)) {/*����Ƿ������ݿ��д��ڣ����������½�һ��*/
		cout << "�û����������ݿⷢ������" << endl;
		return false;
	}
	if (!g_UserMgr.AddUser(row)) {
		cout << "�û��������߶���ʧ��" << endl;
		return false;
	}
	return true;
}

bool CLoginSystem::Logout(string& _strAccount) {
	/*ɾ��map�е�ָ������*/
	
	if (!g_UserMgr.DelUser(_strAccount)) {
		cout << "�ǳ�ʧ��" << endl;
		return false;
	}
	return true;
}