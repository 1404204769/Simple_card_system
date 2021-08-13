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
			m_mapByCardId[pCard->GetCardId()] = pCard.release();
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
	return iterById->second;
}
void CCardMgr::PrintAll() {
	/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	CardMapIter iterById = m_mapByCardId.begin();
	while (iterById != m_mapByCardId.end()) {
		CCard* pCard = iterById->second;
		iterById++;
		if (!pCard) {
			continue;
		}
		const CCardType& CardType = pCard->GetCardTypeData();
		const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tName:" << pCard->GetName() << "\tLev:" << pCard->GetLev()
			<< "\tExp:" << pCard->GetExp() 
			<< "\tHp:" << CardType.GetHp()+CardLevAttrType.GetHp() 
			<< "\tMp:" << CardType.GetMp()+CardLevAttrType.GetMp()
			<< "\tAtk:" << CardType.GetAtk()+CardLevAttrType.GetAtk()<<endl;
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

	if (!pCard->CreateNewCard(m_pUser->GetId(),pCardType)) {
		cout << "�������ݳ�ʼ��ʧ�ܣ����ӿ���ʧ��" << endl;
		return false;
	}

	m_mapByCardId[pCard->GetCardId()] = pCard.release();
	return true;
}
bool CCardMgr::Del(long long int i64CardId) {
	/*������ҿ���������ɾ������*/
	if (i64CardId <= 0) {
		cout << "����ID���Ϸ�" << endl;
		return false;
	}

	CCard* pCard = m_mapByCardId[i64CardId];
	m_mapByCardId[i64CardId] = nullptr;

	if (!pCard) {
		cout << "ɾ��ʧ�ܣ������û�����Ž�����"<< i64CardId <<"���Ŀ���" << endl;
		return false;
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
	bool bRet = true;
	CardMapIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		CCard* pCard = iterByName->second;
		iterByName->second = nullptr;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}	

		if (!pCard->Delete()) {
			cout << "����ɾ��ʧ��" << endl;
			bRet = false;
		}
		delete pCard;
	}
	m_mapByCardId.clear();
	return bRet;
}
void CCardMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	CardMapIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		delete iterByName->second;//���ͷ��ڴ�
		iterByName->second = nullptr;//�ÿ�
		iterByName++;
	}
	m_mapByCardId.clear();
}

