#include "CardMgr.h"
#include "User.h"
using namespace std;
CCardMgr::CCardMgr() {
	/*���캯��*/
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
			cout << "���쿨�ƹ�����ʱ�����û�������" << endl;
			return false;
		}

		m_pUser = pUser;
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
				cout << "�û������еĿ������ͷǷ�,������ʾ" << endl;
				continue;
			}
			if (!pCard->CreateFromDB(row, pCardType)) {
				Log("CCardMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
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
	return true;
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
		const long long int i64CardId = pCard->GetCardId();
		CSkin* pSkin = SkinMgr.GetWearing(i64CardId);
		const CCardType& CardType = pCard->GetCardTypeData();
		const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();

		long long int i64Hp = 0, i64Mp = 0, i64Atk = 0;
		string strSkinName = "��\t";

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
		i64Hp += CardType.GetHp() + CardLevAttrType.GetHp();
		i64Mp += CardType.GetMp() + CardLevAttrType.GetMp();
		i64Atk += CardType.GetAtk() + CardLevAttrType.GetAtk();
		cout << "CardID:" << i64CardId << "\tUserId:" << pCard->GetUserId() << "\tName:" << pCard->GetName() <<"\tSkin:"<<strSkinName
			<< "\tLev:" << pCard->GetLev() << "\tExp:" << pCard->GetExp() << "\tHp:" << i64Hp << "\tMp:" << i64Mp << "\tAtk:" << i64Atk << endl;
	}
}
bool CCardMgr::Add(const CCardType* pCardType) {
	/*����һ��ָ�����͵Ŀ���*/
	if (!m_pUser) 
	{
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
	if (i64CardId <= 0) {
		cout << "����ID���Ϸ�" << endl;
		return false;
	}

	CCard* pCard = nullptr;
	if(m_mapByCardId.count(i64CardId))
		pCard=m_mapByCardId[i64CardId];

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

	m_mapByCardId.erase(i64CardId);
	if(!pCard->Delete())
	{
		delete pCard;
		return false;
	}
	delete pCard;
	return true;
}
bool CCardMgr::DelAll() {
	/*ɾ�����еĿ���*/
	bool ret = true;
	for (auto &iter:m_mapByCardId) {
		CCard* pCard = iter.second;
		if (!pCard) {
			ret = false;
			continue;
		}	
		const long long int i64CardId = pCard->GetCardId();
		CSkinMgr& SkinMgr = m_pUser->GetSkinMgr();
		CSkin* pSkin = SkinMgr.GetWearing(i64CardId);
		const long long int i64SkinId = pSkin->GetSkinId();
		if (pSkin) {
			SkinMgr.Drop(i64CardId, i64SkinId);
		}

		if (!pCard->Delete()) {
			cout << "����ɾ��ʧ��" << endl;
			ret = false;
		}
		delete pCard;
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

