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
		if (!g_CardSystem.Add(strUserAccount, unCardType))
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
		if (!g_CardSystem.Del(strUserAccount, i64CardId))
			cout << "����ɾ��ʧ��" << endl;
		else
			cout << "����ɾ���ɹ�" << endl;
	}break;
	case 5: {
		string strUserAccount;
		cout << "������ָ����ҵ��˻�" << endl;
		getline(cin, strUserAccount);
		if (!g_CardSystem.Show(strUserAccount))
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
		if (!g_CardSystem.ShowAtk(strUserAccount, i64CardId))
			cout << "��ѯ���ָ�����ƹ�����ʧ��" << endl;
		else
			cout << "��ѯ���ָ�����ƹ������ɹ�" << endl;
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
		cout << "��ѡ����:\n1.����\n2.ע��\n3.ָ���������ָ�����Ϳ���\n4.ָ�����ɾ��һ��ָ������\n5.��ѯָ����ҵ����п���\n6.��ѯָ�����ָ�����ƵĹ�����\n7.��ʾ�������������Ϣ" << endl;
		int nchoice = 1;
		cin >> nchoice;
		getchar();
		bContinue = Subject(nchoice);
		system("pause");
	}

	return 0;
}