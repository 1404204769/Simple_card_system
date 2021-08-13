#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	Log("调用了CCardSystem构造函数\n");
}
CCardSystem::~CCardSystem() {
	Log("调用了CCardSystem析构函数\n");
}
bool CCardSystem::Add(const std::string& _strUserAccount,const unsigned int _unCardType) {
	/*为指定玩家增加一副指定的卡牌*/

	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	const CCardType* pCardType = g_CardTypeMgr.Get(_unCardType);
	if (!pCardType) {
		cout << "指定卡牌类型无效" << endl;
		return false;
	}
	if (!CardMgr.Add(pCardType)) {
		return false;
	}
	return true;

}
bool CCardSystem::Del(const std::string& _strUserAccount,const long long int _i64CardId) {
	/*为指定玩家删除指定卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	if (!CardMgr.Del(_i64CardId)) {
		return false;
	}
	return true;
}
bool CCardSystem::Show(const std::string& _strUserAccount) {
	/*打印显示指定用户的所有卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	CardMgr.PrintAll();
	return true;
}
bool CCardSystem::ShowAtk(const std::string& _strUserAccount,const long long int _i64CardId) {
	/*打印显示指定用户的指定卡牌的攻击力*/
	long long int i64UserId = g_UserMgr.GetOnlineId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr &CardMgr = pUser->GetCardMgr();
	CCard* pCard= CardMgr.Get(_i64CardId);
	if (!pCard) {
		cout << "指定卡牌不存在" << endl;
		return false;
	}
	const CCardType &CardType = pCard->GetCardTypeData();
	cout << "指定卡牌攻击力为：" <<CardType.GetAtk() << endl;
	return true;
}