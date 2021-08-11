#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	stringstream strIn;
	strIn << "������CCardSystem���캯��\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCardSystem::~CCardSystem() {
	stringstream strIn;
	strIn << "������CCardSystem��������\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CCardSystem::AddUserCard(std::string& _strUserAccount, unsigned int _unCardType) {
	/*Ϊָ���������һ��ָ���Ŀ���*/

	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(_unCardType);
	if (!pCardType) {
		cout << "ָ������������Ч" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		pCardType = nullptr;
		return false;
	}
	if (!pCardMgr->AddCard(pUser->GetId(), pCardType)) {
		pUser = nullptr;
		pCardMgr = nullptr;
		pCardType = nullptr;
		return false;
	}
	pUser = nullptr;
	pCardMgr = nullptr;
	pCardType = nullptr;
	return true;

}
bool CCardSystem::DelUserCard(std::string& _strUserAccount, std::string& _strCardName) {
	/*Ϊָ�����ɾ��ָ������*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	if (!pCardMgr->DelCardByName(_strCardName)) {
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	pUser = nullptr;
	pCardMgr = nullptr;
	return true;
}
bool CCardSystem::ShowUserCard(std::string& _strUserAccount) {
	/*��ӡ��ʾָ���û������п���*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	pCardMgr->PrintAllCard();
	pUser = nullptr;
	pCardMgr = nullptr;
	return true;
}
bool CCardSystem::ShowUserCardAtk(std::string& _strUserAccount, std::string& _strCardName) {
	/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	CCard* pCard= pCardMgr->GetCardByCardName(_strCardName);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		pCard = nullptr;
		return false;
	}
	const CCardType* pCardType = pCard->GetCardTypeData();
	if (!pCardType) {
		cout << "�޷��鿴ָ����������Ӧ���͵���ϸ��Ϣ" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		pCard = nullptr;
		pCardType = nullptr;
		return false;
	}
	cout << "ָ�����ƹ�����Ϊ��" <<pCardType->GetAtk() << endl;
	pUser = nullptr;
	pCardMgr = nullptr;
	pCard = nullptr;
	pCardType = nullptr;
	return true;
}