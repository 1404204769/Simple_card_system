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
bool Subject(int nchoice) {
	switch (nchoice)
	{
	case 1: {
		string strAccount = "";
		cout << "请输入要登入的账号:";
		getline(cin, strAccount);
		if (!g_LoginSystem.Login(strAccount)) 
			cout << "登入失败，请重试" << endl;
		else
			cout << "登入成功，请继续" << endl;
	} break;
	case 2: {
		string strAccount = "";
		cout << "请输入要注销的账户:";
		getline(cin, strAccount);
		if (!g_LoginSystem.Logout(strAccount))
			cout << "注销失败，请重试" << endl;
		else
			cout << "注销成功，请继续" << endl;
	} break;
	case 3: {
		string strUserAccount;
		unsigned int unCardType;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		cout << "请输入要新增的卡牌的类型" << endl;
		cin >> unCardType;
		getchar();
		if (!g_CardSystem.Add(strUserAccount, unCardType))
			cout << "卡牌新增失败" << endl;
		else
			cout << "卡牌新增成功" << endl;
	} break;
	case 4: {
		string strUserAccount;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		cout << "请输入要删除的卡牌的Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		if (!g_CardSystem.Del(strUserAccount, i64CardId))
			cout << "卡牌删除失败" << endl;
		else
			cout << "卡牌删除成功" << endl;
	}break;
	case 5: {
		string strUserAccount;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		if (!g_CardSystem.Show(strUserAccount))
			cout << "查询玩家所有卡牌失败" << endl;
		else
			cout << "查询玩家所有卡牌成功" << endl;
	}break;
	case 6: {
		string strUserAccount;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		cout << "请输入要查询攻击力的卡牌的Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		if (!g_CardSystem.ShowAtk(strUserAccount, i64CardId))
			cout << "查询玩家指定卡牌攻击力失败" << endl;
		else
			cout << "查询玩家指定卡牌攻击力成功" << endl;
	}break;
	case 7: {
		g_UserMgr.PrintOnline();
	}break;
	default:return false;
		break;
	}
	return true;
}
int main() {
	Init();
	bool bContinue = true;
	while (bContinue) {
		system("cls");
		cout << "请选择功能:\n1.登入\n2.注销\n3.指定玩家新增指定类型卡牌\n4.指定玩家删除一张指定卡牌\n5.查询指定玩家的所有卡牌\n6.查询指定玩家指定卡牌的攻击力\n7.显示所有在线玩家信息" << endl;
		int nchoice = 1;
		cin >> nchoice;
		getchar();
		bContinue = Subject(nchoice);
		system("pause");
	}

	return 0;
}