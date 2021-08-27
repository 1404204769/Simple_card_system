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
bool CCardSystem::CardRankLevUp(const long long int i64UserId, const long long int i64CardId, vector<long long int>&vecConsume) {
	/*为指定玩家的指定卡牌提升阶级*/
	set<long long int>setConsume(vecConsume.begin(),vecConsume.end());
	if (setConsume.size() != vecConsume.size()) {
		cout << "要消耗的卡牌中出现了重复的" << endl;
		return false;
	}

	CUser* const pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CCardMgr& rCardMgr = pUser->GetCardMgr();
	CCard* const pCard = rCardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "要升阶的主体卡牌不存在" << endl;
		return false;
	}

	const unsigned int unRankLev = pCard->GetCardRankLev();

	const CCardRankType* pCardRandType = g_CardRankTypeMgr.Get(unRankLev+1);
	if (!pCardRandType) {
		cout << "此卡牌已经无法升阶" << endl;
		return false;
	}
	const unsigned int unCostNum = pCardRandType->GetCostCardNum();
	const unsigned int unCardType = pCard->GetCardType();

	cout << "此次升阶需要消耗" << unCostNum << "张同类型同阶级卡牌" << endl;

	if (vecConsume.size() != unCostNum) {
		cout << "发生错误，消耗品数量不满足升阶条件" << endl;
		return false;
	}
	cout << "正在逐个检查要消耗的卡牌ID的合法性" << endl;
	for (const auto i64ConCardId : vecConsume) {
		CCard* const pCard = rCardMgr.Get(i64ConCardId);
		if (!pCard) {
			cout << "卡牌ID:"<< i64ConCardId <<" 是无效ID" << endl;
			return false;
		}
		if (!IsLowValue(pCard)) {
			cout << "要消耗的卡牌是高价值的，请慎重考虑" << endl;
			return false;
		}
		if (pCard->GetCardType() != unCardType) {
			cout << "卡牌ID:" << i64ConCardId << " 所对应的卡牌的类型与升阶主体卡牌不同" << endl;
			return false;
		}
		if (pCard->GetCardRankLev() != unRankLev) {
			cout << "卡牌ID:" << i64ConCardId << " 所对应的卡牌的阶级与升阶主体卡牌不同" << endl;
			return false;
		}
	}
	for (const auto i64ConCardId : vecConsume) {
		if (!rCardMgr.Del(i64ConCardId)) {
			cout << "消耗品ID:"<< i64ConCardId <<"消耗失败，升阶失败，已消耗的卡牌已报废" << endl;
			return false;
		}
	}
	return pCard->RankUp();
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
	const CCardRankType& CardRankType = pCard->GetCardRankTypeData();
	Atk += CardType.GetAtk() + CardLevAttrType.GetAtk()+ CardRankType.GetAtk();
	if (pSkin) {
		unsigned int SkinType = pSkin->GetSkinType();
		const CSkinType* pSkinType = g_SkinTypeMgr.Get(SkinType);
		if (!pSkinType) {
			cout << "皮肤类型数据发生错误" << endl;
			return false;
		}
		Atk += pSkinType->GetAtk();
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	CCard::EquipIter iterBegin = pCard->GetEquipIterBegin();
	CCard::EquipIter iterEnd = pCard->GetEquipIterEnd();
	unsigned int unPos = 0;
	while (iterBegin != iterEnd) {
		const long long int i64EquipId = iterBegin->second;
		unPos++;
		if (i64EquipId != 0) {
			CEquip* pEquip = EquipMgr.Get(i64EquipId);
			if (pEquip) {
				const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
				if (!pEquipType) {
					cout << "装备类型数据发生错误" << endl;
					continue;
				}
				Atk += pEquipType->GetAtk();
			}
		}
		iterBegin++;
	}
	cout << "指定卡牌攻击力为：" <<Atk<< endl;
	return true;
}


bool CCardSystem::SkinWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*指定用户指定卡牌穿指定皮肤*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Wear(i64CardId, i64SkinId);
}
bool CCardSystem::SkinDrop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*指定用户指定卡牌脱指定皮肤*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Drop(i64CardId, i64SkinId);
}
bool CCardSystem::SkinAdd(const long long int i64UserId, const unsigned int unSkinType) {
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
bool CCardSystem::SkinShow(const long long int i64UserId) {
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

bool CCardSystem::IsLowValue(CCard* const pCard) const {
	/*判断卡牌是否是低价值的*/
	if (!pCard) {
		return false;
	}
	if (pCard->GetLev() > 1)
		return false;
	return true;
}


bool CCardSystem::EquipWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64EquipId) {
	/*指定用户指定卡牌穿指定装备*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	/*指定某张卡牌穿上某个装备*/
	if (i64EquipId <= 0) {
		cout << "装备ID不合法" << endl;
		return false;
	}
	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	CCardMgr& CardMgr = pUser->GetCardMgr();

	CEquip* pEquip = EquipMgr.Get(i64EquipId);
	if (!pEquip) {
		cout << "穿戴失败，该玩家没有这张ID为：" << i64EquipId << " 的装备" << endl;
		return false;
	}
	if (EquipMgr.GetWearCardId(i64EquipId)) {
		cout << "穿戴失败，此装备已被穿戴" << endl;
		return false;
	}

	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "此卡牌不存在" << endl;
		return false;
	}
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
	if (!pCard->Wear(i64EquipId, pEquipType)) {
		//Card将装备位上的数据清零
		cout << "卡牌上的装备位穿戴失败" << endl;
		return false;
	}
	return EquipMgr.AddEquipCardMap(i64EquipId, i64CardId);
}
bool CCardSystem::EquipDrop(const long long int i64UserId, const long long int  i64EquipId) {
	/*指定用户脱指定装备*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	CCardMgr& CardMgr = pUser->GetCardMgr();

	if (i64EquipId <= 0) {
		cout << "皮肤ID不合法" << endl;
		return false;
	}
	CEquip* pEquip = EquipMgr.Get(i64EquipId);
	if (!pEquip) {
		cout << "脱下失败，该玩家没有这张ID为：" << i64EquipId << " 的装备" << endl;
		return false;
	}

	const long long int i64CardId = EquipMgr.GetWearCardId(i64EquipId);
	if (i64CardId==0) {
		cout << "此装备还未被穿戴" << endl;
		return false;
	}

	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "此卡牌不存在" << endl;
		return false;
	}

	const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
	if (!pEquipType) {
		cout << "装备类型静态配置错误" << endl;
		return false;
	}
	const unsigned int unPos = pEquipType->GetPos();
	if (!pCard->Drop(unPos)) {
		//Card将装备位上的数据清零
		cout << "卡牌上的装备位脱下失败" << endl;
	}
	return EquipMgr.DelEquipCardMap(i64EquipId);    
}
bool CCardSystem::EquipAdd(const long long int i64UserId, const unsigned int unEquipType) {
	/*指定用户获得指定类型装备*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}
	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(unEquipType);
	if (!pEquipType) {
		cout << "指定装备类型无效" << endl;
		return false;
	}
	return EquipMgr.AddNew(pEquipType);
}
bool CCardSystem::EquipShow(const long long int i64UserId) {
	/*展示用户拥有的所有装备*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "指定用户未登入，请登入后操作" << endl;
		return false;
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	EquipMgr.PrintAll();
	return true;
}