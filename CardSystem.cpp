#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	Log("调用了CCardSystem构造函数\n");
}
CCardSystem::~CCardSystem() {
	Log("调用了CCardSystem析构函数\n");
}
bool CCardSystem::AddCard(const std::string& strUserAccount,const unsigned int unCardType) {
	/*为指定玩家增加一副指定的卡牌*/

	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
	const CCardType* pCardType = g_CardTypeMgr.Get(unCardType);
	if (!pCardType) {
		cout << "指定卡牌类型无效" << endl;
		return false;
	}
	if (!CardMgr.Add(pCardType)) {
		return false;
	}
	return true;

}
bool CCardSystem::DelCard(const std::string& strUserAccount,const long long int i64CardId) {
	/*为指定玩家删除指定卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
	if (!CardMgr.Del(i64CardId)) {
		return false;
	}
	return true;
}
bool CCardSystem::CardLevUp(const std::string& strUserAccount, const long long int i64CardId, const unsigned int unLev) {
	/*为指定玩家的指定卡牌提升指定等级*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr& CardMgr = pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "指定卡牌不存在" << endl;
		return false;
	}
	if (!pCard->LevelUp(unLev)) {
		cout << "升级过程出错" << endl;
		return false;
	}
	return true;
}
bool CCardSystem::CardRankLevUp(const long long int i64UserId, const long long int i64CardId) {
	/*为指定玩家的指定卡牌提升阶级*/

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr& CardMgr = pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "要升阶的主体卡牌不存在" << endl;
		return false;
	}

	const unsigned int unRankLev = pCard->GetCardRankLev();

	const CCardRankType* CardRandType = g_CardRankTypeMgr.Get(unRankLev+1);
	if (!CardRandType) {
		cout << "此卡牌已经无法升阶" << endl;
		return false;
	}
	const unsigned int unCostNum = CardRandType->GetCostCardNum();
	const unsigned int unCardType = pCard->GetCardType();
	vector<long long int>vecConsume;

	cout << "此次升阶需要消耗" << unCostNum << "张同类型同阶级卡牌" << endl;
	cout << "请逐个输入要消耗的卡牌ID（若要取消升阶请输入0）" << endl;
	for (unsigned int i = 0; i < unCostNum; i++) {
		cout << "请输入第"<<i+1<<"个消耗品的ID: " << endl;
		long long int i64ConCardId;
		cin >> i64ConCardId;
		if (i64ConCardId == 0) {
			cout << "检测到取消升阶的操作，正在取消升阶" << endl;
			vecConsume.clear();
			return false;
		}
		CCard* pCard = CardMgr.Get(i64ConCardId);
		if (!pCard) {
			cout << "输入的卡牌ID是无效ID，请重新输入" << endl;
			i--;
			continue;
		}
		if (pCard->GetCardType() != unCardType) {
			cout << "输入的卡牌ID所对应的卡牌的类型与升阶主体卡牌不同，请重新输入" << endl;
			i--;
			continue;
		}
		if (pCard->GetCardRankLev() != unRankLev) {
			cout << "输入的卡牌ID所对应的卡牌的阶级与升阶主体卡牌不同，请重新输入" << endl;
			i--;
			continue;
		}
		vecConsume.push_back(i64ConCardId);
	}
	cout << "消耗品输入完毕，现在开始准备升阶" << endl;
	if (vecConsume.size() != unCostNum) {
		cout << "发生错误，消耗品数量不满足升阶条件" << endl;
		vecConsume.clear();
		return false;
	}
	for (auto iter : vecConsume) {
		if (!CardMgr.Del(iter)) {
			cout << "消耗品ID:"<<iter<<"消耗失败，升阶失败，已消耗的卡牌已报废" << endl;
			return false;
		}
	}
	pCard->RankUp();
	return true;
}
bool CCardSystem::ShowCard(const std::string& strUserAccount) {
	/*打印显示指定用户的所有卡牌*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
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
bool CCardSystem::ShowCardAtk(const std::string& strUserAccount,const long long int i64CardId) {
	/*打印显示指定用户的指定卡牌的攻击力*/
	long long int i64UserId = g_UserMgr.GetOnlineId(strUserAccount);
	if (i64UserId == 0) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	CCardMgr& CardMgr = pUser->GetCardMgr();
	CCard* pCard= CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "指定卡牌不存在" << endl;
		return false;
	}
	CSkin* pSkin = SkinMgr.GetWearing(pCard->GetCardId());
	long long int Atk = 0;
	const CCardType &CardType = pCard->GetCardTypeData();
	const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();
	Atk += CardType.GetAtk() + CardLevAttrType.GetAtk();
	if (pSkin) {
		unsigned int SkinType = pSkin->GetSkinType();
		const CSkinType* pSkinType = g_SkinTypeMgr.Get(SkinType);
		if (!pSkinType) {
			cout << "皮肤类型数据发生错误" << endl;
			return false;
		}
		Atk += pSkinType->GetAtk();
	}
	cout << "指定卡牌攻击力为：" <<Atk<< endl;
	return true;
}


bool CCardSystem::Wear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*指定用户指定卡牌穿指定皮肤*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Wear(i64CardId, i64SkinId);
}
bool CCardSystem::Drop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*指定用户指定卡牌脱指定皮肤*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Drop(i64CardId, i64SkinId);
}
bool CCardSystem::AddSkin(const long long int i64UserId, const unsigned int unSkinType) {
	/*指定用户获得指定类型指定皮肤*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	const CSkinType* pSkinType = g_SkinTypeMgr.Get(unSkinType);
	if (!pSkinType) {
		cout << "指定皮肤类型无效" << endl;
		return false;
	}
	return SkinMgr.AddNew(pSkinType);
}
bool CCardSystem::ShowSkin(const long long int i64UserId) {
	/*展示用户拥有的所有皮肤*/

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	SkinMgr.PrintAll();
	return true;
}