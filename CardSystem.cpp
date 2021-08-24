#include "CardSystem.h"
using namespace std;
CCardSystem::CCardSystem() {
	Log("������CCardSystem���캯��\n");
}
CCardSystem::~CCardSystem() {
	Log("������CCardSystem��������\n");
}
bool CCardSystem::AddCard(const std::string& strUserAccount,const unsigned int unCardType) {
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
bool CCardSystem::DelCard(const std::string& strUserAccount,const long long int i64CardId) {
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
bool CCardSystem::CardRankLevUp(const long long int i64UserId, const long long int i64CardId, vector<long long int>&vecConsume) {
	/*Ϊָ����ҵ�ָ�����������׼�*/
	set<long long int>setConsume(vecConsume.begin(),vecConsume.end());
	if (setConsume.size() != vecConsume.size()) {
		cout << "Ҫ���ĵĿ����г������ظ���" << endl;
		return false;
	}

	CUser* const pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CCardMgr& rCardMgr = pUser->GetCardMgr();
	CCard* const pCard = rCardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "Ҫ���׵����忨�Ʋ�����" << endl;
		return false;
	}

	const unsigned int unRankLev = pCard->GetCardRankLev();

	const CCardRankType* pCardRandType = g_CardRankTypeMgr.Get(unRankLev+1);
	if (!pCardRandType) {
		cout << "�˿����Ѿ��޷�����" << endl;
		return false;
	}
	const unsigned int unCostNum = pCardRandType->GetCostCardNum();
	const unsigned int unCardType = pCard->GetCardType();

	cout << "�˴�������Ҫ����" << unCostNum << "��ͬ����ͬ�׼�����" << endl;

	if (vecConsume.size() != unCostNum) {
		cout << "������������Ʒ������������������" << endl;
		return false;
	}
	cout << "����������Ҫ���ĵĿ���ID�ĺϷ���" << endl;
	for (const auto i64ConCardId : vecConsume) {
		CCard* const pCard = rCardMgr.Get(i64ConCardId);
		if (!pCard) {
			cout << "����ID:"<< i64ConCardId <<" ����ЧID" << endl;
			return false;
		}
		if (!IsLowValue(pCard)) {
			cout << "Ҫ���ĵĿ����Ǹ߼�ֵ�ģ������ؿ���" << endl;
			return false;
		}
		if (pCard->GetCardType() != unCardType) {
			cout << "����ID:" << i64ConCardId << " ����Ӧ�Ŀ��Ƶ��������������忨�Ʋ�ͬ" << endl;
			return false;
		}
		if (pCard->GetCardRankLev() != unRankLev) {
			cout << "����ID:" << i64ConCardId << " ����Ӧ�Ŀ��ƵĽ׼����������忨�Ʋ�ͬ" << endl;
			return false;
		}
	}
	for (const auto i64ConCardId : vecConsume) {
		if (!rCardMgr.Del(i64ConCardId)) {
			cout << "����ƷID:"<< i64ConCardId <<"����ʧ�ܣ�����ʧ�ܣ������ĵĿ����ѱ���" << endl;
			return false;
		}
	}
	return pCard->RankUp();
}
bool CCardSystem::ShowCard(const std::string& strUserAccount) {
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
bool CCardSystem::ShowCardAtk(const std::string& strUserAccount,const long long int i64CardId) {
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
	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	CCardMgr& CardMgr = pUser->GetCardMgr();
	CCard* pCard= CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "ָ�����Ʋ�����" << endl;
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
			cout << "Ƥ���������ݷ�������" << endl;
			return false;
		}
		Atk += pSkinType->GetAtk();
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	const long long int i64EquipHat = pCard->GetPosHat();
	if (i64EquipHat != 0) {
		CEquip* pEquip = EquipMgr.Get(i64EquipHat);
		if (!pEquip) {
			cout << "ͷ��װ���޷��ҵ�,������������" << endl;
		}
		else {
			const CEquipType* pEquipHatType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
			if (pEquipHatType) {
				Atk += pEquipHatType->GetAtk();
			}
		}
	}
	const long long int i64EquipArmour = pCard->GetPosArmour();

	if (i64EquipArmour != 0) {
		CEquip* pEquip = EquipMgr.Get(i64EquipArmour);
		if (!pEquip) {
			cout << "�¼ײ�λװ���޷��ҵ�,������������" << endl;
		}
		else {
			const CEquipType* pEquipArmourType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
			if (pEquipArmourType) {
				Atk += pEquipArmourType->GetAtk();
			}
		}
	}
	const long long int i64EquipShoes = pCard->GetPosShoes();
	if (i64EquipShoes != 0) {
		CEquip* pEquip = EquipMgr.Get(i64EquipShoes);
		if (!pEquip) {
			cout << "Ь�Ӳ�λװ���޷��ҵ�,������������" << endl;
		}
		else {
			const CEquipType* pEquipShoesType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
			if (pEquipShoesType) {
				Atk += pEquipShoesType->GetAtk();
			}
		}
	}
	cout << "ָ�����ƹ�����Ϊ��" <<Atk<< endl;
	return true;
}


bool CCardSystem::SkinWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*ָ���û�ָ�����ƴ�ָ��Ƥ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Wear(i64CardId, i64SkinId);
}
bool CCardSystem::SkinDrop(const long long int i64UserId, const long long int i64CardId, const long long int  i64SkinId) {
	/*ָ���û�ָ��������ָ��Ƥ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	return SkinMgr.Drop(i64CardId, i64SkinId);
}
bool CCardSystem::SkinAdd(const long long int i64UserId, const unsigned int unSkinType) {
	/*ָ���û����ָ������ָ��Ƥ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	const CSkinType* pSkinType = g_SkinTypeMgr.Get(unSkinType);
	if (!pSkinType) {
		cout << "ָ��Ƥ��������Ч" << endl;
		return false;
	}
	return SkinMgr.AddNew(pSkinType);
}
bool CCardSystem::SkinShow(const long long int i64UserId) {
	/*չʾ�û�ӵ�е�����Ƥ��*/

	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = pUser->GetSkinMgr();
	SkinMgr.PrintAll();
	return true;
}

bool CCardSystem::IsLowValue(CCard* const pCard) const {
	/*�жϿ����Ƿ��ǵͼ�ֵ��*/
	if (!pCard) {
		return false;
	}
	if (pCard->GetLev() > 1)
		return false;
	return true;
}


bool CCardSystem::EquipWear(const long long int i64UserId, const long long int i64CardId, const long long int  i64EquipId) {
	/*ָ���û�ָ�����ƴ�ָ��װ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	return EquipMgr.Wear(i64CardId, i64EquipId);
}
bool CCardSystem::EquipDrop(const long long int i64UserId, const long long int  i64EquipId) {
	/*ָ���û���ָ��װ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	return EquipMgr.Drop(i64EquipId);
}
bool CCardSystem::EquipAdd(const long long int i64UserId, const unsigned int unEquipType) {
	/*ָ���û����ָ������װ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}
	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(unEquipType);
	if (!pEquipType) {
		cout << "ָ��װ��������Ч" << endl;
		return false;
	}
	return EquipMgr.AddNew(pEquipType);
}
bool CCardSystem::EquipShow(const long long int i64UserId) {
	/*չʾ�û�ӵ�е�����װ��*/
	CUser* pUser = g_UserMgr.Get(i64UserId);
	if (!pUser) {
		cout << "ָ���û�δ���룬���������" << endl;
		return false;
	}

	CEquipMgr& EquipMgr = pUser->GetEquipMgr();
	EquipMgr.PrintAll();
	return true;
}