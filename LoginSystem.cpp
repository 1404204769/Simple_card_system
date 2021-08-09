#include "LoginSystem.h"
using namespace std;
using namespace mysqlpp;
CLoginSystem::CLoginSystem() {
	cout << "������CLoginSystem�Ĺ��캯��" << endl;
}
CLoginSystem::~CLoginSystem() {
	cout << "������CLoginSystem����������" << endl;
}
bool CLoginSystem::Login() {
	/*����Ϊ�����account,����CUserMgrSearchInMap&SearchInDB���ܣ��������ڷ��ص���ʧ��*/
	string strAccount = "", strName = "";
	cout << "������Ҫ������˺�:";
	getline(cin, strAccount);
	
	long long int i64Id = g_UserMgr.getOnlineUserId(strAccount);
	if (i64Id != 0) {
		cout << "����ʧ��,���û��ѵ��룡" << endl;
		return false;
	}
	mysqlpp::Row row;
	if (!g_UserMgr.CheckExistInDB(row, strAccount)) {/*����Ƿ������ݿ��д��ڣ����������½�һ��*/
		cout << "�û����������ݿⷢ������" << endl;
		return false;
	}
	if (!g_UserMgr.AddUser(row)) {
		cout << "�û��������߶���ʧ��" << endl;
		return false;
	}
	return true;
}

bool CLoginSystem::Logout() {
	/*ɾ��map�е�ָ������*/
	string strAccount = "";
	cout << "������Ҫע�����˻�:";
	getline(cin, strAccount);
	if (!g_UserMgr.DelUser(strAccount)) {
		cout << "�ǳ�ʧ��" << endl;
		return false;
	}
	return true;
}