#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	stringstream strIn;
	strIn << "调用了CCardSystem构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCardSystem::~CCardSystem() {
	stringstream strIn;
	strIn << "调用了CCardSystem析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CCardSystem::AddUserCard(std::string& _strUserAccount, unsigned int _unCardType) {
	/*为指定玩家增加一副指定的卡牌*/

	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "用户卡牌管理器获取失败" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(_unCardType);
	if (!pCardType) {
		cout << "指定卡牌类型无效" << endl;
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
	/*为指定玩家删除指定卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "用户卡牌管理器获取失败" << endl;
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
	/*打印显示指定用户的所有卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "用户卡牌管理器获取失败" << endl;
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
	/*打印显示指定用户的指定卡牌的攻击力*/
	long long int i64UserId = g_UserMgr.GetOnlineUserId(_strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CUser* pUser = g_UserMgr.GetUser(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		pUser = nullptr;
		return false;
	}
	CCardMgr* pCardMgr = pUser->GetCardMgr();
	if (!pCardMgr) {
		cout << "用户卡牌管理器获取失败" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		return false;
	}
	CCard* pCard= pCardMgr->GetCardByCardName(_strCardName);
	if (!pCard) {
		cout << "指定卡牌不存在" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		pCard = nullptr;
		return false;
	}
	const CCardType* pCardType = pCard->GetCardTypeData();
	if (!pCardType) {
		cout << "无法查看指定卡牌所对应类型的详细信息" << endl;
		pUser = nullptr;
		pCardMgr = nullptr;
		pCard = nullptr;
		pCardType = nullptr;
		return false;
	}
	cout << "指定卡牌攻击力为：" <<pCardType->GetAtk() << endl;
	pUser = nullptr;
	pCardMgr = nullptr;
	pCard = nullptr;
	pCardType = nullptr;
	return true;
}