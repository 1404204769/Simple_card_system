#include "CardMgr.h"
#include "EquipMgr.h"
#include "User.h"
#include<array>
using namespace std;
CCardMgr::CCardMgr() {
	/*���캯��*/
	m_bInit = false;
	Log("������CCardMgr���캯��\n");
}
CCardMgr::~CCardMgr() {
	/*��������*/
	Log("������CCardMgr��������\n");

	Free();
	m_pUser = nullptr;
}
bool CCardMgr::Init(CUser* pUser) {
	/*�����ڲ��������ݿ��ȡ����*/
	try
	{
		if (!pUser) 
		{
			Log("���쿨�ƹ�����ʱ�����û�������\n");
			return false;
		}

		m_pUser = pUser;
		CEquipMgr& EquipMgr = m_pUser->GetEquipMgr();
		if (!EquipMgr.IsInit()) {
			Log("װ����������δ��ʼ��\n");
			return false;
		}
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log( "CCardMgr::Init()\n");
		if (!*pQuery) {
			Log("Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from d_card where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardMgr::Init()  �����ݿ��ѯ�û���������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CCardMgr::Init()  �����ݿ��ѯ�û��������ݳɹ�\n");

		while (row = res.fetch_row()) {
			unique_ptr<CCard> pCard(new CCard());
			if (!pCard) {
				Log("CCardMgr::Init()  ����ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.Get(row["card_type"]);
			if (!pCardType) {
				Log("�û������еĿ������ͷǷ�,������ʾ\n");
				continue;
			}

			if (!pCard->CreateFromDB(row, pCardType)) {
				Log("CCardMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}

			const long long int i64CardId = row["id"];
			long long int  i64EquipId;
			string stdPos="";
			for (int i = 1; i <= CCard::EquipPosMax; i++) {
				pCard->GetFieldNamePos(stdPos,i);
				i64EquipId = row[stdPos.c_str()];
				if (i64EquipId == 0)
					continue;
				if (EquipMgr.GetWearCardId(i64EquipId)==0) {
					//��װ��δ������װ������
					EquipMgr.AddEquipCardMap(i64EquipId, i64CardId);
				}
				else {
					pCard->Drop(i);//����Ӧװ��λ�ϵ�װ�����·�ֹ���������Ƴ�ͻ
				}

			}

			const long long int i64CardInt = pCard->GetCardId();
			m_mapByCardId.insert({ i64CardInt,pCard.release() });
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardMgr::Init()  Query error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardMgr::Init()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string( er.retrieved )+ ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCardMgr::Init()  Error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardMgr::Init()  Error: " +string( er.what())+ "\n");
		return false;
	}
	m_bInit = true;
	return m_bInit;
}

bool CCardMgr::IsInit()const {
	/*����Ƿ��ʼ��*/
	return m_bInit;
}
CCard* CCardMgr::Get(const long long int _i64CardId) {
	/*���ݿ������ƻ�ȡ��������*/
	CardMapIter iterById = m_mapByCardId.find(_i64CardId);
	if (iterById == m_mapByCardId.end()) {
		cout << "Do not Find CardId:" << _i64CardId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
void CCardMgr::PrintAll() {
	/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	if (!m_pUser) {
		cout << "���ƹ��������û�ʧ��" << endl;
		return;
	}
	CSkinMgr& SkinMgr = m_pUser->GetSkinMgr();

	for(auto &iter:m_mapByCardId) {

		CCard* pCard = iter.second;
		if (!pCard) {
			continue;
		}

		const CCardType& CardType = pCard->GetCardTypeData();
		const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();
		const CCardRankType& CardRankType = pCard->GetCardRankTypeData();
		long long int i64Hp = CardType.GetHp() + CardLevAttrType.GetHp() + CardRankType.GetHp();
		long long int i64Mp = CardType.GetMp() + CardLevAttrType.GetMp() + CardRankType.GetMp();
		long long int i64Atk = CardType.GetAtk() + CardLevAttrType.GetAtk() + CardRankType.GetAtk();

		const long long int i64CardId = pCard->GetCardId();

		string strSkinName = "��";
		CSkin* pSkin = SkinMgr.GetWearing(i64CardId);
		if (pSkin) {
			unsigned int SkinType = pSkin->GetSkinType();
			const CSkinType* pSkinType = g_SkinTypeMgr.Get(SkinType);
			if (!pSkinType) {
				cout << "Ƥ���������ݷ�������" << endl;
				continue;
			}
			i64Hp += pSkinType->GetHp();
			i64Mp += pSkinType->GetMp();
			i64Atk += pSkinType->GetAtk();
			strSkinName = pSkinType->GetName();
		}

		array<string, CCard::EquipPosMax> strszEquipName;
		strszEquipName.fill("��");
		CEquipMgr& EquipMgr = m_pUser->GetEquipMgr();
		unsigned int unPos=0;
		for (CCard::EquipIter iterBegin = pCard->GetEquipIterBegin(), iterEnd = pCard->GetEquipIterEnd(); iterBegin != iterEnd; iterBegin++,unPos++) {
			const long long int i64EquipId = iterBegin->second;
			if (i64EquipId == 0)
				continue;
			CEquip* pEquip = EquipMgr.Get(i64EquipId);
			if (!pEquip)
				continue;
			const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
			if (!pEquipType)
				continue;
			i64Hp += pEquipType->GetHp();
			i64Mp += pEquipType->GetMp();
			i64Atk += pEquipType->GetAtk();
			strszEquipName[unPos] = pEquipType->GetName();
		}
		
		cout << "CardID:" << i64CardId 
			<< "\n\tUserId:" << pCard->GetUserId() << "\tName:" << pCard->GetName()<< "\tLev:" << pCard->GetLev() 
			<< "\tRank_Lev:" << pCard->GetCardRankLev() << "\tExp:" << pCard->GetExp()
			<<"\tHp:" << i64Hp << "\tMp:" << i64Mp << "\tAtk:" << i64Atk
			<< "\n\tSkin:" << strSkinName;
		for (int i = 0; i < CCard::EquipPosMax; i++) {
			cout << "\tװ��" << i + 1 << ": " << strszEquipName.at(i);
		}cout << endl;
	}
}
bool CCardMgr::Add(const CCardType* pCardType) {
	/*����һ��ָ�����͵Ŀ���*/
	if (!m_pUser) {
		cout << "�˿��ƹ��������û�ʧ����" << endl;
		return false;
	}

	if (!pCardType) {
		cout << "����������Ч" << endl;
		return false;
	}

	unique_ptr<CCard> pCard(new CCard());
	if (!pCard) {
		cout << "Cardʵ�廯ʧ��" << endl;
		return false;
	}

	const long long int i64UserId = m_pUser->GetId();
	if (!pCard->CreateNewCard(i64UserId,pCardType)) {
		cout << "�������ݳ�ʼ��ʧ�ܣ����ӿ���ʧ��" << endl;
		return false;
	}

	const long long int i64CardId = pCard->GetCardId();
	m_mapByCardId.insert({ i64CardId, pCard.release() });
	return true;
}
bool CCardMgr::Del(long long int i64CardId) {
	/*������ҿ���������ɾ������*/
	if (!m_pUser) {
		cout << "���ƹ��������û�ʧ����" << endl;
		return false;
	}

	unique_ptr<CCard> pCard(Get(i64CardId));
	if (!pCard) {
		cout << "ɾ��ʧ�ܣ������û�����Ž�����"<< i64CardId <<"���Ŀ���" << endl;
		return false;
	}

	CSkinMgr& SkinMgr = m_pUser->GetSkinMgr();
	CSkin* pSkin = SkinMgr.GetWearing(i64CardId);
	if (pSkin) {
		const long long int i64SkinId = pSkin->GetSkinId();
		SkinMgr.Drop(i64CardId, i64SkinId);
	}

	CEquipMgr& EquipMgr = m_pUser->GetEquipMgr();
	for (CCard::EquipIter iterBegin = pCard->GetEquipIterBegin(), iterEnd = pCard->GetEquipIterEnd(); iterBegin != iterEnd; iterBegin++) {
		const long long int i64EquipId = iterBegin->second;
		if (i64EquipId == 0)//װ��λ����װ������
			continue;

		CEquip* pEquip = EquipMgr.Get(i64EquipId);
		if (!pEquip)//װ�����Ʋ�����
			continue;

		EquipMgr.DelEquipCardMap(i64EquipId);//����װ������
	}

	m_mapByCardId.erase(i64CardId);
	return pCard->Delete();
}
bool CCardMgr::DelAll() {
	/*ɾ�����еĿ���*/
	if (!m_pUser) {
		cout << "���ƹ��������û�ʧ����" << endl;
		return false;
	}
	bool ret = true;
	CSkinMgr& SkinMgr = m_pUser->GetSkinMgr();
	for (auto &iter:m_mapByCardId) {
		unique_ptr<CCard> pCard(iter.second);
		if (!pCard) {
			ret = false;
			continue;
		}	
		const long long int i64CardId = pCard->GetCardId();
		CSkin* pSkin = SkinMgr.GetWearing(i64CardId);
		const long long int i64SkinId = pSkin->GetSkinId();
		if (pSkin) {
			SkinMgr.Drop(i64CardId, i64SkinId);
		}

		CEquipMgr& EquipMgr = m_pUser->GetEquipMgr();
		for (CCard::EquipIter iterBegin = pCard->GetEquipIterBegin(), iterEnd = pCard->GetEquipIterEnd(); iterBegin != iterEnd; iterBegin++) {
			const long long int i64EquipId = iterBegin->second;
			if (i64EquipId == 0)//װ��λ����װ������
				continue;

			CEquip* pEquip = EquipMgr.Get(i64EquipId);
			if (!pEquip)//װ�����Ʋ�����
				continue;

			EquipMgr.DelEquipCardMap(i64EquipId);//����װ������
		}

		if (!pCard->Delete()) {
			cout << "����ɾ��ʧ��" << endl;
			ret = false;
		}
	}
	m_mapByCardId.clear();
	return ret;
}
void CCardMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto &iter:m_mapByCardId) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapByCardId.clear();
}

