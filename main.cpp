#include<iostream>
#include "User.h"
#include "UserMgr.h"
using namespace std;
CDB g_DB;
CUserMgr g_UserMgr;
const string strDB("test");
const string strServer("localhost");
const string strUser("root");
const string strPassword("root");
const int nPort = 3306;
int main() {
	int nchoice=1;
	string strInput = "";
	g_DB.InitConnect(strDB, strServer, strUser, strPassword, nPort);
	while (nchoice) {
		cout << "��ѡ����(1.���� 2.ע�� 3.����)" << endl;
		cin >> nchoice;
		getchar();
		switch (nchoice)
		{
		case 1: {
			string strAccount="", strName = "";
			cout << "������Ҫ������˺�:";
			getline(cin, strAccount);
			if (!g_UserMgr.AddUser(strAccount)) {
				cout << "����ʧ��" << endl;
			}
			else
				cout << "����ɹ�" << endl;
		} break;
		case 2: {
			string strAccount = "";
			cout << "������Ҫע�����˻�:";
			getline(cin, strAccount);
			g_UserMgr.ReduceUser(strAccount);
		} break;
		case 3: {
			long long int i64Id;
			cout << "������Ҫ�������˺�ID:";
			cin >> i64Id;
			CUser *pUser=g_UserMgr.getUser(i64Id);
			if (!pUser) {
				cout << "���˻������ڣ������²���" << endl;
				break;
			}
			cout << "������Ҫ���ĵȼ�:";
			unsigned int unLev;
			cin >> unLev;
			pUser->setLev(pUser->getLev()+unLev);
		} break;
		default:nchoice = 0;
			break;
		}
	}
}