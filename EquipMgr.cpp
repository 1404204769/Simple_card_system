#include "EquipMgr.h"
#include "CardMgr.h"
#include "User.h"
using namespace std;
CEquipMgr::CEquipMgr() {
	/*���캯��*/
	Log("������CEquipMgr���캯��\n");
}
CEquipMgr::~CEquipMgr() {
	/*��������*/
	Log("������CEquipMgr��������\n");

	Free();
	m_pUser = nullptr;
}

bool CEquipMgr::Init(CUser* pUser) {
	/*�����ڲ��������ݿ��ȡ����*/
	try
	{
		if (!pUser)
		{
			cout << "����װ��������ʱ�����û�������" << endl;
			return false;
		}

		m_pUser = pUser;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CEquipMgr::Init()\n");
		if (!*pQuery) {
			Log("Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from d_equip where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CEquipMgr::Init()  �����ݿ��ѯ�û�װ������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CEquipMgr::Init()  �����ݿ��ѯ�û�װ�����ݳɹ�\n");
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CEquip> pEquip(new CEquip());
			if (!pEquip) {
				Log("CEquipMgr::Init()  װ��ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}

			const CEquipType* pEquipType = g_EquipTypeMgr.Get(row["equip_type"]);
			if (!pEquipType) {
				cout << "�û������е�װ�����ͷǷ�,������ʾ" << endl;
				continue;
			}
			if (!pEquip->CreateFromDB(row, pEquipType)) {
				Log("CEquipMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!Add(pEquip.release())) {
				cout << "��װ����ID:" << pEquip->GetEquipId() << "�������ڴ�ʧ��" << endl;
			}
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquipMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquipMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquipMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquipMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	m_bInit = true;
	return m_bInit;
}
bool CEquipMgr::IsInit() {
	/*�����Ƿ��ʼ��*/
	return m_bInit;
}
CEquip* CEquipMgr::Get(const long long int i64EquipId) {
	/*����װ��ID��ȡƤ������*/
	EquipMapIter iterById = m_map.find(i64EquipId);
	if (iterById == m_map.end()) {
		cout << "Do not Find EquipId:" << i64EquipId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
bool CEquipMgr::AddNew(const CEquipType* pEquipType) {
	/*����һ��ָ�����͵�װ��*/
	if (!m_pUser)
	{
		cout << "��װ�����������û�ʧ����" << endl;
		return false;
	}

	if (!pEquipType) {
		cout << "װ��������Ч" << endl;
		return false;
	}
	unique_ptr<CEquip> pEquip(new CEquip());
	if (!pEquip) {
		cout << "Equipʵ�廯ʧ��" << endl;
		return false;
	}

	const long long int i64UserId = m_pUser->GetId();
	if (!pEquip->CreateNewEquip(i64UserId, pEquipType)) {
		cout << "װ�����ݳ�ʼ��ʧ�ܣ�����װ��ʧ��" << endl;
		return false;
	}
	return Add(pEquip.release());
}

bool CEquipMgr::DelExist(const long long int i64EquipId) {
	/*ɾ��һ�����е�װ��*/
	return Del(i64EquipId);
}
void CEquipMgr::PrintAll() {
	/*��ӡ��ʾӵ�е�����װ��������*/
	for (auto& iter : m_map) {
		CEquip* pEquip = iter.second;
		if (!pEquip) {
			continue;
		}
		const unsigned int unEquipType = pEquip->GetEquipType();
		const CEquipType* pEquipType = g_EquipTypeMgr.Get(unEquipType);
		if (!pEquipType) {
			cout << "װ�����Ͳ�����" << endl;
			continue;
		}
		cout << "EquipID:" << pEquip->GetEquipId() << "\tUserId:" << pEquip->GetUserId()
			<< "\tEquipType:" << pEquipType->GetId() << "\tEquipName:" << pEquipType->GetName()
			<< "\tEquipAtk:" << pEquipType->GetAtk() << "\tEquipHp:" << pEquipType->GetHp() 
			<< "\tEquipMp:" << pEquipType->GetMp() << endl;
	}
}
bool CEquipMgr::Wear(const long long int i64CardId, const long long int i64EquipId) {
	/*ָ��ĳ�ſ��ƴ���ĳ��װ��*/
	if (i64EquipId <= 0) {
		cout << "װ��ID���Ϸ�" << endl;
		return false;
	}
	CEquip* pEquip = nullptr;
	if (m_map.count(i64EquipId))
		pEquip = m_map[i64EquipId];

	if (!pEquip) {
		cout << "����ʧ�ܣ������û������IDΪ��" << i64EquipId << " ��װ��" << endl;
		return false;
	}
	if (m_mapEquipCard.count(i64EquipId) == 1) {
		cout << "��װ���ѱ�����(ID: "<<m_mapEquipCard[i64EquipId]<<")����" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	if (!CardMgr.IsInit()) {
		cout << "���ƹ�������δ��ʼ��" << endl;
		return false;
	}
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "�˿��Ʋ�����" << endl;
		return false;
	}
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
	const unsigned int unPos = pEquipType->GetPos();
	if (!pCard->Wear(i64EquipId, unPos)) {
		//Card��װ��λ�ϵ���������
		cout << "�����ϵ�װ��λ����ʧ��" << endl;
		return false;
	}
	return AddEquipCardMap(i64EquipId, i64CardId);
}
bool CEquipMgr::Drop(const long long int i64EquipId) {
	/*ָ��ĳ�ſ�������ĳ��װ��*/
	if (i64EquipId <= 0) {
		cout << "Ƥ��ID���Ϸ�" << endl;
		return false;
	}
	CEquip* pEquip = nullptr;
	if (m_map.count(i64EquipId))
		pEquip = m_map[i64EquipId];

	if (!pEquip) {
		cout << "����ʧ�ܣ������û������IDΪ��" << i64EquipId << " ��װ��" << endl;
		return false;
	}
	if (m_mapEquipCard.count(i64EquipId)==0) {
		cout << "��װ����δ������" << endl;
		return false;
	}
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
	const long long int i64CardId = m_mapEquipCard[i64EquipId];
	const unsigned int unPos = pEquipType->GetPos();
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	if (!CardMgr.IsInit()) {
		cout << "���ƹ�������δ��ʼ��" << endl;
		return false;
	}
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "�˿��Ʋ�����" << endl;
		return false;
	}
	if (!pCard->Drop(i64EquipId, unPos)) {
		//Card��װ��λ�ϵ���������
		cout << "�����ϵ�װ��λ����ʧ��" << endl;
	}
	return DelEquipCardMap(i64EquipId);
}
bool CEquipMgr::AddEquipCardMap(const long long int i64EquipId, const long long int i64CardId) {
	/*��������������Ӳ���*/
	if (m_mapEquipCard.count(i64EquipId)==1&& m_mapEquipCard[i64EquipId]!= i64CardId) {
		cout << "��װ���ѱ����ƣ�ID:" << m_mapEquipCard[i64EquipId] <<"������" << endl;
		return false;
	}
	m_mapEquipCard.insert({ i64EquipId ,i64CardId });
	return true;
}
bool CEquipMgr::DelEquipCardMap(const long long int i64EquipId) {
	/*�����������ɾ������*/
	if (m_mapEquipCard.count(i64EquipId)==0) {
		cout << "��װ��δ������" << endl;
		return false;
	}
	m_mapEquipCard.erase(i64EquipId);
	return true;
}
bool CEquipMgr::Add(CEquip* pEquip) {
	/*�������Ӳ���*/
	if (!m_pUser) {
		cout << "��װ�����������û�ʧ����" << endl;
		return false;
	}
	if (!pEquip) {
		cout << "�����װ��ָ��Ϊ��" << endl;
		return false;
	}
	const long long int i64EquipId = pEquip->GetEquipId();
	if (m_map.count(i64EquipId)) {
		cout << "��װ��ID�Ѵ���" << endl;
		return false;
	}
	m_map.insert({ i64EquipId ,pEquip });//����Ƥ��ʵ��
	return true;
}
bool CEquipMgr::Del(const long long int i64EquipId) {
	/*����װ��ID��ɾ������*/
	if (i64EquipId <= 0) {
		cout << "Ƥ��ID���Ϸ�" << endl;
		return false;
	}
	CEquip* pEquip = nullptr;
	if (m_map.count(i64EquipId))
		pEquip = m_map[i64EquipId];

	if (!pEquip) {
		cout << "ɾ��ʧ�ܣ������û������IDΪ��" << i64EquipId << " ��װ��" << endl;
		return false;
	}
	const CEquipType* pEquipType = g_EquipTypeMgr.Get(pEquip->GetEquipType());
	if (!pEquipType) {
		cout << "��װ�����ͷǷ�" << endl;
		m_map[i64EquipId]->Delete();
		m_map.erase(i64EquipId);
		m_mapEquipCard.erase(i64EquipId);
		return false;
	}

	if (m_mapEquipCard.count(i64EquipId)) {
		cout << "��װ���ѱ��������������¸�װ��" << endl;
		const long long int i64CardId = m_mapEquipCard[i64EquipId];
		const unsigned int unPos = pEquipType->GetPos();
		CCardMgr& CardMgr = m_pUser->GetCardMgr();
		if (!CardMgr.IsInit()) {
			cout << "���ƹ�������δ��ʼ��" << endl;
			return false;
		}
		CCard* pCard = CardMgr.Get(i64CardId);

		if (pCard&&!pCard->Drop(i64EquipId,unPos )) {
			//Card��װ��λ�ϵ���������
			cout << "�����ϵ�װ��λ����ʧ��" << endl;
		}
		DelEquipCardMap(i64EquipId);
	}

	if (!pEquip->Delete())
	{
		delete pEquip;
		return false;
	}
	delete pEquip;
	return true;
}
void CEquipMgr::Free(){
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto& iter : m_map) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_map.clear();
	m_mapEquipCard.clear();
}