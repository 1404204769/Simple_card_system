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
bool Init() {
	g_DB.InitConnect(strDB, strServer, strUser, strPassword, nPort);
	if (!g_CardTypeMgr.Init()) {
		cout << "卡牌类型静态数据加载失败" << endl;
		return false;
	}
	if (!g_SkinTypeMgr.Init()) {
		cout << "皮肤类型静态数据加载失败" << endl;
		return false;
	}
	g_SkinTypeMgr.PrintAll();
	if (!g_EquipTypeMgr.Init()) {
		cout << "装备类型静态数据加载失败" << endl;
		return false;
	}
	g_EquipTypeMgr.PrintAll();
	if (!g_CardRankTypeMgr.Init()) {
		cout << "卡牌阶级静态数据加载失败" << endl;
		return false;
	}
	g_CardRankTypeMgr.Debug_PrintAll();
	if (!g_CardLevAttrTypeMgr.Init()) {
		cout << "卡牌等级静态数据加载失败" << endl;
		return false;
	}
	g_CardLevAttrTypeMgr.Debug_PrintAll();
	system("pause");
	return true;
}
void Free() {
	g_UserMgr.Free();
	g_CardTypeMgr.Free();
	g_SkinTypeMgr.Free();
	g_EquipTypeMgr.Free();
	g_CardRankTypeMgr.Free();
	g_CardLevAttrTypeMgr.Free();

}
void CardRankLevUp();/*卡牌升级*/
bool Subject(int nchoice);
int main() {
	bool bContinue = Init();
	while (bContinue) {
		system("cls");
		cout << "请选择功能:\n1.登入\n2.注销\n\n3.指定玩家新增指定类型卡牌\n4.指定玩家删除一张指定卡牌\n5.查询指定玩家的所有卡牌\n6.查询指定玩家指定卡牌的攻击力\n7.显示所有在线玩家信息\n8.指定玩家指定卡牌升级\n\n9.给玩家卡牌穿上皮肤\n10.从玩家卡牌脱下皮肤\n11.给玩家增加皮肤\n12.显示玩家拥有的所有皮肤\n\n13.指定玩家指定卡牌升阶\n\n14.给玩家卡牌穿上装备\n15.从玩家卡牌脱下装备\n16.给玩家增加装备\n17.显示玩家拥有的所有装备\n" << endl;
		int nchoice = 1;
		cin >> nchoice;
		getchar();
		bContinue = Subject(nchoice);
		system("pause");
	}
	Free();
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
		if (!g_CardSystem.SkinWear(i64UserId, i64CardId, i64SkinId))
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
		if (!g_CardSystem.SkinDrop(i64UserId, i64CardId, i64SkinId))
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
		if (!g_CardSystem.SkinAdd(i64UserId, unSkinType))
			cout << "皮肤获取失败" << endl;
		else
			cout << "皮肤获取成功" << endl;
	}break;
	case 12: {
		long long int i64UserId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		if (!g_CardSystem.SkinShow(i64UserId))
			cout << "皮肤展示失败" << endl;
		else
			cout << "皮肤展示成功" << endl;
	}break;
	case 13: {
		CardRankLevUp();
	}break;
	case 14: {
		long long int i64UserId, i64CardId, i64EquipId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要穿戴的卡牌的Id" << endl;
		cin >> i64CardId;
		cout << "请输入要穿戴的装备的Id" << endl;
		cin >> i64EquipId;
		if (!g_CardSystem.EquipWear(i64UserId, i64CardId, i64EquipId))
			cout << "装备穿戴失败" << endl;
		else
			cout << "装备穿戴成功" << endl;
	}break;
	case 15: {
		long long int i64UserId, i64EquipId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要脱下的装备的Id" << endl;
		cin >> i64EquipId;
		if (!g_CardSystem.EquipDrop(i64UserId, i64EquipId))
			cout << "装备脱下失败" << endl;
		else
			cout << "装备脱下成功" << endl;
	}break;
	case 16: {
		long long int i64UserId;
		unsigned int unEquipType;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		cout << "请输入要获取的装备的类型" << endl;
		cin >> unEquipType;
		if (!g_CardSystem.EquipAdd(i64UserId, unEquipType))
			cout << "装备获取失败" << endl;
		else
			cout << "装备获取成功" << endl;
	}break;
	case 17: {
		long long int i64UserId;
		cout << "请输入指定玩家的Id" << endl;
		cin >> i64UserId;
		if (!g_CardSystem.EquipShow(i64UserId))
			cout << "装备展示失败" << endl;
		else
			cout << "装备展示成功" << endl;
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