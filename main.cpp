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
		cout << "请选择功能(1.登入 2.注销 3.升级)" << endl;
		cin >> nchoice;
		getchar();
		switch (nchoice)
		{
		case 1: {
			string strAccount="", strName = "";
			cout << "请输入要登入的账号:";
			getline(cin, strAccount);
			if (!g_UserMgr.AddUser(strAccount)) {
				cout << "登入失败" << endl;
			}
			else
				cout << "登入成功" << endl;
		} break;
		case 2: {
			string strAccount = "";
			cout << "请输入要注销的账户:";
			getline(cin, strAccount);
			g_UserMgr.ReduceUser(strAccount);
		} break;
		case 3: {
			long long int i64Id;
			cout << "请输入要升级的账号ID:";
			cin >> i64Id;
			CUser *pUser=g_UserMgr.getUser(i64Id);
			if (!pUser) {
				cout << "此账户不存在，请重新操作" << endl;
				break;
			}
			cout << "请输入要升的等级:";
			unsigned int unLev;
			cin >> unLev;
			pUser->setLev(pUser->getLev()+unLev);
		} break;
		default:nchoice = 0;
			break;
		}
	}
}