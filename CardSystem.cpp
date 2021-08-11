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
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		return false;
	}
	const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(_unCardType);
	if (!pCardType) {
		cout << "ָ������������Ч" << endl;
		return false;
	}
	if (!pCardMgr->AddCard(pUser->GetId(), pCardType)) {
		return false;
	}
	return true;

}
bool CCardSystem::DelUserCard(std::string& _strUserAccount, long long int _i64CardId) {
	/*Ϊָ�����ɾ��ָ������*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		return false;
	}
	if (!pCardMgr->DelCardById(_i64CardId)) {
		return false;
	}
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
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		return false;
	}
	pCardMgr->PrintAllCard();
	return true;
}
bool CCardSystem::ShowUserCardAtk(std::string& _strUserAccount, long long int _i64CardId) {
	/*��ӡ��ʾָ���û���ָ�����ƵĹ�����*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "�û����ƹ�������ȡʧ��" << endl;
		return false;
	}
	CCard* pCard= pCardMgr->GetCardByCardId(_i64CardId);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
		return false;
	}
	const CCardType* pCardType = pCard->GetCardTypeData();
	if (!pCardType) {
		cout << "�޷��鿴ָ����������Ӧ���͵���ϸ��Ϣ" << endl;
		return false;
	}
	cout << "ָ�����ƹ�����Ϊ��" <<pCardType->GetAtk() << endl;
	return true;
}