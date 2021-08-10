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
		cout << "��ѡ����:\n1.����\n2.ע��\n3.ָ���������ָ�����Ϳ���\n4.ָ�����ɾ��һ��ָ������\n5.��ѯָ����ҵ����п���\n6.��ѯָ�����ָ�����ƵĹ�����" << endl;
		cin >> nchoice;
		getchar();
		switch (nchoice)
		{
		case 1: {
			string strAccount ="";
			cout << "������Ҫ������˺�:";
			getline(cin, strAccount);
			g_LoginSystem.Login(strAccount);
		} break;
		case 2: {
			string strAccount = "";
			cout << "������Ҫע�����˻�:";
			getline(cin, strAccount);
			g_LoginSystem.Logout(strAccount);
		} break;
		case 3: {
			string strUserAccount;
			unsigned int unCardType;
			cout << "������ָ����ҵ��˻�" << endl;
			getline(cin, strUserAccount);
			cout << "������Ҫ�����Ŀ��Ƶ�����" << endl;
			cin >> unCardType;
			getchar();
			if (!g_CardSystem.AddUserCard(strUserAccount, unCardType)) 
				cout << "��������ʧ��" << endl;
			else
				cout << "���������ɹ�" << endl;
		} break;
		case 4: {
			string strUserAccount;
			cout << "������ָ����ҵ��˻�" << endl;
			getline(cin, strUserAccount);
			cout << "������Ҫɾ���Ŀ��Ƶ�����" << endl;
			string strCardName;
			getline(cin, strCardName);
			if (!g_CardSystem.DelUserCard(strUserAccount, strCardName))
				cout << "����ɾ��ʧ��" << endl;
			else
				cout << "����ɾ���ɹ�" << endl;
		}break;
		case 5: {
			string strUserAccount;
			cout << "������ָ����ҵ��˻�" << endl;
			getline(cin, strUserAccount);
			if (!g_CardSystem.ShowUserCard(strUserAccount))
				cout << "��ѯ������п���ʧ��" << endl;
			else
				cout << "��ѯ������п��Ƴɹ�" << endl;
		}break;
		case 6: {
			string strUserAccount;
			cout << "������ָ����ҵ��˻�" << endl;
			getline(cin, strUserAccount);
			cout << "������Ҫ��ѯ�������Ŀ��Ƶ�����" << endl;
			string strCardName;
			getline(cin, strCardName);
			if (!g_CardSystem.ShowUserCardAtk(strUserAccount,strCardName))
				cout << "��ѯ���ָ�����ƹ�����ʧ��" << endl;
			else
				cout << "��ѯ���ָ�����ƹ������ɹ�" << endl;
		}break;
		default:nchoice = 0;
			break;
		}
	}
	return 0;
}