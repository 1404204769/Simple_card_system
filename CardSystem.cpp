#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	Log("������CCardSystem���캯��\n");
}
CCardSystem::~CCardSystem() {
	Log("������CCardSystem��������\n");
}
bool CCardSystem::Add(const std::string& strUserAccount,const unsigned int unCardType) {
	/*Ϊָ���������һ��ָ���Ŀ���*/

	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
	const CCardType* pCardType = g_CardTypeMgr.Get(unCardType);
	if (!pCardType) {
		cout << "ָ������������Ч" << endl;
		return false;
	}
	if (!CardMgr.Add(pCardType)) {
		return false;
	}
	return true;

}
bool CCardSystem::Del(const std::string& strUserAccount,const long long int i64CardId) {
	/*Ϊָ�����ɾ��ָ������*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
	if (!CardMgr.Del(i64CardId)) {
		return false;
	}
	return true;
}
bool CCardSystem::CardLevUp(const std::string& strUserAccount, const long long int i64CardId, const unsigned int unLev) {
	/*Ϊָ����ҵ�ָ����������ָ���ȼ�*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr& CardMgr = pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
		return false;
	}
	if (!pCard->LevelUp(unLev)) {
		cout << "�������̳���" << endl;
		return false;
	}
	return true;
}
bool CCardSystem::Show(const std::string& strUserAccount) {
	/*��ӡ��ʾָ���û������п���*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
bool CCardSystem::ShowAtk(const std::string& strUserAccount,const long long int i64CardId) {
	/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
	CCard* pCard= CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
		return false;
	}
	const CCardType &CardType = pCard->GetCardTypeData();
	const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();
	cout << "ָ�����ƹ�����Ϊ��" <<CardType.GetAtk()+CardLevAttrType.GetAtk() << endl;
	return true;
}