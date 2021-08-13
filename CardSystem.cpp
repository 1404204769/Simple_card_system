#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	Log("������CCardSystem���캯��\n");
}
CCardSystem::~CCardSystem() {
	Log("������CCardSystem��������\n");
}
bool CCardSystem::Add(const std::string& _strUserAccount,const unsigned int _unCardType) {
	/*Ϊָ���������һ��ָ���Ŀ���*/

	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	const CCardType* pCardType = g_CardTypeMgr.Get(_unCardType);
	if (!pCardType) {
		cout << "ָ������������Ч" << endl;
		return false;
	}
	if (!CardMgr.Add(pCardType)) {
		return false;
	}
	return true;

}
bool CCardSystem::Del(const std::string& _strUserAccount,const long long int _i64CardId) {
	/*Ϊָ�����ɾ��ָ������*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	if (!CardMgr.Del(_i64CardId)) {
		return false;
	}
	return true;
}
bool CCardSystem::Show(const std::string& _strUserAccount) {
	/*��ӡ��ʾָ���û������п���*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	CardMgr.PrintAll();
	return true;
}
bool CCardSystem::ShowAtk(const std::string& _strUserAccount,const long long int _i64CardId) {
	/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	CCard* pCard= CardMgr.Get(_i64CardId);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
		return false;
	}
	const CCardType &CardType = pCard->GetCardTypeData();
	cout << "ָ�����ƹ�����Ϊ��" <<CardType.GetAtk() << endl;
	return true;
}