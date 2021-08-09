#include<iostream>
#include "User.h"
#include "UserMgr.h"
#include "LoginSystem.h"
#include "UserLevelSystem.h"
using namespace std;
CDB g_DB;
CUserMgr g_UserMgr;
CLoginSystem g_LoginSystem;
CUserLevelSystem g_UserLevelSystem;
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
		cout << "��ѡ����(1.���� 2.ע�� 3.���� 4.��ʾ����������ҵ���Ϣ)" << endl;
		cin >> nchoice;
		getchar();
		switch (nchoice)
		{
		case 1: {
			g_LoginSystem.Login();
		} break;
		case 2: {
			g_LoginSystem.Logout();
		} break;
		case 3: {
			g_UserLevelSystem.LevelUp();
		} break;
		case 4: {
			g_UserMgr.PrintOnlineUser();
		}break;
		default:nchoice = 0;
			break;
		}
	}
}