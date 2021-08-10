#include<iostream>
#include "User.h"
#include "UserMgr.h"
#include "CardMgr.h"
#include "CardSystem.h"
#include "CardTypeMgr.h"
#include "LoginSystem.h"
#include "UserLevelSystem.h"
using namespace std;
CDB g_DB;
CUserMgr g_UserMgr;
CCardMgr g_CardMgr;
CCardTypeMgr g_CardTypeMgr;
CLoginSystem g_LoginSystem;
CUserLevelSystem g_UserLevelSystem;
CCardSystem g_CardSystem;
const string strDB("test");
const string strServer("localhost");
const string strUser("root");
const string strPassword("root");
const int nPort = 3306;
void Init() {
	g_DB.InitConnect(strDB, strServer, strUser, strPassword, nPort);
	g_CardTypeMgr.Init();
}
int main() {
	int nchoice=1;
	Init();
	while (nchoice) {
		cout << "请选择功能:\n1.登入\n2.注销\n3.指定玩家新增指定类型卡牌\n4.指定玩家删除一张指定卡牌\n5.查询指定玩家的所有卡牌\n6.查询指定玩家指定卡牌的攻击力" << endl;
		cin >> nchoice;
		getchar();
		switch (nchoice)
		{
		case 1: {
			string strAccount ="";
			cout << "请输入要登入的账号:";
			getline(cin, strAccount);
			g_LoginSystem.Login(strAccount);
		} break;
		case 2: {
			string strAccount = "";
			cout << "请输入要注销的账户:";
			getline(cin, strAccount);
			g_LoginSystem.Logout(strAccount);
		} break;
		case 3: {
			string strUserAccount;
			unsigned int unCardType;
			cout << "请输入指定玩家的账户" << endl;
			getline(cin, strUserAccount);
			cout << "请输入要新增的卡牌的类型" << endl;
			cin >> unCardType;
			getchar();
			if (!g_CardSystem.AddUserCard(strUserAccount, unCardType)) 
				cout << "卡牌新增失败" << endl;
			else
				cout << "卡牌新增成功" << endl;
		} break;
		case 4: {
			string strUserAccount;
			cout << "请输入指定玩家的账户" << endl;
			getline(cin, strUserAccount);
			cout << "请输入要删除的卡牌的名称" << endl;
			string strCardName;
			getline(cin, strCardName);
			if (!g_CardSystem.DelUserCard(strUserAccount, strCardName))
				cout << "卡牌删除失败" << endl;
			else
				cout << "卡牌删除成功" << endl;
		}break;
		case 5: {
			string strUserAccount;
			cout << "请输入指定玩家的账户" << endl;
			getline(cin, strUserAccount);
			if (!g_CardSystem.ShowUserCard(strUserAccount))
				cout << "查询玩家所有卡牌失败" << endl;
			else
				cout << "查询玩家所有卡牌成功" << endl;
		}break;
		case 6: {
			string strUserAccount;
			cout << "请输入指定玩家的账户" << endl;
			getline(cin, strUserAccount);
			cout << "请输入要查询攻击力的卡牌的名称" << endl;
			string strCardName;
			getline(cin, strCardName);
			if (!g_CardSystem.ShowUserCardAtk(strUserAccount,strCardName))
				cout << "查询玩家指定卡牌攻击力失败" << endl;
			else
				cout << "查询玩家指定卡牌攻击力成功" << endl;
		}break;
		default:nchoice = 0;
			break;
		}
	}
	return 0;
}