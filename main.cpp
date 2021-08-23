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
void CardRankLevUp();/*��������*/
bool Subject(int nchoice);
int main() {
	Init();
	bool bContinue = true;
	while (bContinue) {
		system("cls");
		cout << "��ѡ����:\n1.����\n2.ע��\n\n3.ָ���������ָ�����Ϳ���\n4.ָ�����ɾ��һ��ָ������\n5.��ѯָ����ҵ����п���\n6.��ѯָ�����ָ�����ƵĹ�����\n7.��ʾ�������������Ϣ\n8.ָ�����ָ����������\n\n9.����ҿ��ƴ���Ƥ��\n10.����ҿ�������Ƥ��\n11.���������Ƥ��\n12.��ʾ���ӵ�е�����Ƥ��\n\n13.ָ�����ָ����������" << endl;
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
		cout << "������Ҫ������˺�:";
		getline(cin, strAccount);
		if (!g_LoginSystem.Login(strAccount))
			cout << "����ʧ�ܣ�������" << endl;
		else
			cout << "����ɹ��������" << endl;
	} break;
	case 2: {
		string strAccount = "";
		cout << "������Ҫע�����˻�:";
		getline(cin, strAccount);
		if (!g_LoginSystem.Logout(strAccount))
			cout << "ע��ʧ�ܣ�������" << endl;
		else
			cout << "ע���ɹ��������" << endl;
	} break;
	case 3: {
		string strUserAccount;
		unsigned int unCardType;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		cout << "������Ҫ�����Ŀ��Ƶ�����" << endl;
		cin >> unCardType;
		getchar();
		if (!g_CardSystem.AddCard(strUserAccount, unCardType))
			cout << "��������ʧ��" << endl;
		else
			cout << "���������ɹ�" << endl;
	} break;
	case 4: {
		string strUserAccount;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		cout << "������Ҫɾ���Ŀ��Ƶ�Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		if (!g_CardSystem.DelCard(strUserAccount, i64CardId))
			cout << "����ɾ��ʧ��" << endl;
		else
			cout << "����ɾ���ɹ�" << endl;
	}break;
	case 5: {
		string strUserAccount;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		if (!g_CardSystem.ShowCard(strUserAccount))
			cout << "��ѯ������п���ʧ��" << endl;
		else
			cout << "��ѯ������п��Ƴɹ�" << endl;
	}break;
	case 6: {
		string strUserAccount;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		cout << "������Ҫ��ѯ�������Ŀ��Ƶ�Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		if (!g_CardSystem.ShowCardAtk(strUserAccount, i64CardId))
			cout << "��ѯ���ָ�����ƹ�����ʧ��" << endl;
		else
			cout << "��ѯ���ָ�����ƹ������ɹ�" << endl;
	}break;
	case 7: {
		g_UserMgr.PrintOnline();
	}break;
	case 8: {
		string strUserAccount;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		cout << "������Ҫ�����Ŀ��Ƶ�Id" << endl;
		long long int i64CardId;
		cin >> i64CardId;
		cout << "������Ҫ�����ĵȼ�" << endl;
		unsigned int unLev;
		cin >> unLev;
		if (!g_CardSystem.CardLevUp(strUserAccount, i64CardId, unLev))
			cout << "��������ʧ��" << endl;
		else
			cout << "���������ɹ�" << endl;
	}break;
	case 9: {
		long long int i64UserId, i64CardId, i64SkinId;
		cout << "������ָ����ҵ�Id" << endl;
		cin >> i64UserId;
		cout << "������Ҫ�����Ŀ��Ƶ�Id" << endl;
		cin >> i64CardId;
		cout << "������Ҫ������Ƥ����Id" << endl;
		cin >> i64SkinId;
		if (!g_CardSystem.Wear(i64UserId, i64CardId, i64SkinId))
			cout << "Ƥ������ʧ��" << endl;
		else
			cout << "Ƥ�������ɹ�" << endl;
	}break;
	case 10: {
		long long int i64UserId, i64CardId, i64SkinId;
		cout << "������ָ����ҵ�Id" << endl;
		cin >> i64UserId;
		cout << "������Ҫ����Ƥ���Ŀ���Id" << endl;
		cin >> i64CardId;
		cout << "������Ҫ���µ�Ƥ����Id" << endl;
		cin >> i64SkinId;
		if (!g_CardSystem.Drop(i64UserId, i64CardId, i64SkinId))
			cout << "Ƥ������ʧ��" << endl;
		else
			cout << "Ƥ�����³ɹ�" << endl;
	}break;
	case 11: {
		long long int i64UserId;
		unsigned int unSkinType;
		cout << "������ָ����ҵ�Id" << endl;
		cin >> i64UserId;
		cout << "������Ҫ��ȡ��Ƥ��������" << endl;
		cin >> unSkinType;
		if (!g_CardSystem.AddSkin(i64UserId, unSkinType))
			cout << "Ƥ����ȡʧ��" << endl;
		else
			cout << "Ƥ����ȡ�ɹ�" << endl;
	}break;
	case 12: {
		long long int i64UserId;
		cout << "������ָ����ҵ�Id" << endl;
		cin >> i64UserId;
		if (!g_CardSystem.ShowSkin(i64UserId))
			cout << "Ƥ��չʾʧ��" << endl;
		else
			cout << "Ƥ��չʾ�ɹ�" << endl;
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
	cout << "������ָ����ҵ�Id" << endl;
	cin >> i64UserId;
	cout << "������Ҫ���׵Ŀ���Id" << endl;
	cin >> i64CardId;
	cout << "������Ҫ���ĵĿ��Ƶ�����" << endl;
	cin >> nNum;
	vector<long long int>vecConmuse;
	for (int i = 0; i < nNum; i++) {
		cout << "�������" << i + 1 << "������Ʒ��ID: " << endl;
		cin >> i64CardConId;
		vecConmuse.push_back(i64CardConId);
	}
	if (!g_CardSystem.CardRankLevUp(i64UserId, i64CardId,vecConmuse))
		cout << "��������ʧ��" << endl;
	else
		cout << "�������׳ɹ�" << endl;
}