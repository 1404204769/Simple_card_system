#include<iostream>
#include "User.h"
#include "UserMgr.h"
#include "CardMgr.h"
#include "CardSystem.h"
#include "CardTypeMgr.h"
#include "LoginSystem.h"
#include "SkinTypeMgr.h"
#include "EquipTypeMgr.h"
#include "UserLevelSystem.h"
#include "CardRankTypeMgr.h"
#include "CardLevAttrTypeMgr.h"
using namespace std;
CDB g_DB;
CUserMgr g_UserMgr;
CCardSystem g_CardSystem;
CLoginSystem g_LoginSystem;
CCardTypeMgr g_CardTypeMgr;
CSkinTypeMgr g_SkinTypeMgr;
CEquipTypeMgr g_EquipTypeMgr;
CUserLevelSystem g_UserLevelSystem;
CCardRankTypeMgr g_CardRankTypeMgr;
CCardLevAttrTypeMgr g_CardLevAttrTypeMgr;
const string strDB("test");
const string strServer("localhost");
const string strUser("root");
const string strPassword("root");
const int nPort = 3306;
void Init() {
	g_DB.InitConnect(strDB, strServer, strUser, strPassword, nPort);
	g_CardTypeMgr.Init();
	g_SkinTypeMgr.Init();
	g_SkinTypeMgr.PrintAll();
	g_EquipTypeMgr.Init();
	g_EquipTypeMgr.PrintAll();
	g_CardRankTypeMgr.Init();
	g_CardRankTypeMgr.Debug_PrintAll();
	g_CardLevAttrTypeMgr.Init();
	g_CardLevAttrTypeMgr.Debug_PrintAll();
	system("pause");
}
void CardRankLevUp();/*卡牌升级*/
bool Subject(int nchoice);
int main() {
	Init();
	bool bContinue = true;
	while (bContinue) {
		system("cls");
		cout << "请选择功能:\n1.登入\n2.注销\n\n3.指定玩家新增指定类型卡牌\n4.指定玩家删除一张指定卡牌\n5.查询指定玩家的所有卡牌\n6.查询指定玩家指定卡牌的攻击力\n7.显示所有在线玩家信息\n8.指定玩家指定卡牌升级\n\n9.给玩家卡牌穿上皮肤\n10.从玩家卡牌脱下皮肤\n11.给玩家增加皮肤\n12.显示玩家拥有的所有皮肤\n\n13.指定玩家指定卡牌升阶" << endl;
		int nchoice = 1;
		cin >> nchoice;
		getchar();
		bContinue = Subject(nchoice);
		system("pause");
	}

	return 0;
}
bool Subject(int nchoice) {
	switch (nchoice)
	{
	case 0:return false; break;
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
		if (!g_CardSystem.AddCard(strUserAccount, unCardType))
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
		if (!g_CardSystem.DelCard(strUserAccount, i64CardId))
			cout << "卡牌删除失败" << endl;
		else
			cout << "卡牌删除成功" << endl;
	}break;
	case 5: {
		string strUserAccount;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		if (!g_CardSystem.ShowCard(strUserAccount))
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
		if (!g_CardSystem.ShowCardAtk(strUserAccount, i64CardId))
			cout << "查询玩家指定卡牌攻击力失败" << endl;
		else
			cout << "查询玩家指定卡牌攻击力成功" << endl;
	}break;
	case 7: {
		g_UserMgr.PrintOnline();
	}break;
	case 8: {
		string strUserAccount;
		cout << "请输入指定玩家的账户" << endl;
		getline(cin, strUserAccount);
		cout << "请输入要升级的卡牌的Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		cout << "请输入要提升的等级" << endl;
		unsigned int unLev;
		cin >> unLev;
		if (!g_CardSystem.CardLevUp(strUserAccount, i64CardId, unLev))
			cout << "卡牌升级失败" << endl;
		else
			cout << "卡牌升级成功" << endl;
	}break;
	case 9: {
		long long int i64UserId, i64CardId, i64SkinId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要穿戴的卡牌的Id" << endl;
		cin >> i64CardId;
		cout << "请输入要穿戴的皮肤的Id" << endl;
		cin >> i64SkinId;
		if (!g_CardSystem.Wear(i64UserId, i64CardId, i64SkinId))
			cout << "皮肤穿戴失败" << endl;
		else
			cout << "皮肤穿戴成功" << endl;
	}break;
	case 10: {
		long long int i64UserId, i64CardId, i64SkinId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要脱下皮肤的卡牌Id" << endl;
		cin >> i64CardId;
		cout << "请输入要脱下的皮肤的Id" << endl;
		cin >> i64SkinId;
		if (!g_CardSystem.Drop(i64UserId, i64CardId, i64SkinId))
			cout << "皮肤脱下失败" << endl;
		else
			cout << "皮肤脱下成功" << endl;
	}break;
	case 11: {
		long long int i64UserId;
		unsigned int unSkinType;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要获取的皮肤的类型" << endl;
		cin >> unSkinType;
		if (!g_CardSystem.AddSkin(i64UserId, unSkinType))
			cout << "皮肤获取失败" << endl;
		else
			cout << "皮肤获取成功" << endl;
	}break;
	case 12: {
		long long int i64UserId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		if (!g_CardSystem.ShowSkin(i64UserId))
			cout << "皮肤展示失败" << endl;
		else
			cout << "皮肤展示成功" << endl;
	}break;
	case 13: {
		CardRankLevUp();
	}break;
	default:break;
	}
	return true;
}
void CardRankLevUp() {
	long long int i64UserId, i64CardId,i64CardConId;
	int nNum;
	cout << "请输入指定玩家的Id" << endl;
	cin >> i64UserId;
	cout << "请输入要升阶的卡牌Id" << endl;
	cin >> i64CardId;
	cout << "请输入要消耗的卡牌的数量" << endl;
	cin >> nNum;
	vector<long long int>vecConmuse;
	for (int i = 0; i < nNum; i++) {
		cout << "请输入第" << i + 1 << "个消耗品的ID: " << endl;
		cin >> i64CardConId;
		vecConmuse.push_back(i64CardConId);
	}
	if (!g_CardSystem.CardRankLevUp(i64UserId, i64CardId,vecConmuse))
		cout << "卡牌升阶失败" << endl;
	else
		cout << "卡牌升阶成功" << endl;
}