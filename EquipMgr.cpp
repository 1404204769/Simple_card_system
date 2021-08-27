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
				return false;
			}
			if (!pEquip->CreateFromDB(row, pEquipType)) {
				Log("CEquipMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const long long i64EquipId = pEquip->GetEquipId();
			if (!Add(pEquip.release())) {
				cout << "��װ����ID:" << i64EquipId << "�������ڴ�ʧ��" << endl;
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
		Log( "Do not Find EquipId:"+i64EquipId );
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
long long int CEquipMgr::GetWearCardId(const long long int i64EquipId) {
	/*�ж�ĳ��װ���Ƿ񱻴�*/
	EquipCardMapIter iterById = m_mapEquipCard.find(i64EquipId);
	if (iterById == m_mapEquipCard.end()) {
		Log("��װ��(ID:"+to_string(i64EquipId)+")Ŀǰ��δ������");
		return 0;
	}
	Log("��װ��(ID:"+to_string( iterById->first )+")�ѱ�����(ID: " + to_string( iterById->second )+")����" );
	return iterById->second;
}

bool CEquipMgr::Add(CEquip* pEquip) {
	/*�������Ӳ���*/
	unique_ptr<CEquip>upEquip(pEquip);
	if (!upEquip) {
		cout << "�����װ��ָ��Ϊ��" << endl;
		return false;
	}

	const long long int i64EquipId = upEquip->GetEquipId();
	if (Get(i64EquipId)) {
		cout << "��װ��ID�Ѵ���" << endl;
		return false;
	}

	m_map.insert({ i64EquipId ,upEquip.release() });//����Ƥ��ʵ��
	return true;
}
bool CEquipMgr::AddNew(const CEquipType* pEquipType) {
	/*����һ��ָ�����͵�װ��*/
	if (!m_pUser){
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
bool CEquipMgr::AddEquipCardMap(const long long int i64EquipId, const long long int i64CardId) {
	/*��������������Ӳ���*/
	if (!Get(i64EquipId)) {
		return false;
	}
	const long long int i64WearCardId = GetWearCardId(i64EquipId);
	if (i64WearCardId!=0) {
		cout << "��װ���ѱ����ƣ�ID:" << i64WearCardId << "������" << endl;
		return false;
	}
	m_mapEquipCard.insert({ i64EquipId ,i64CardId });
	return true;
}

bool CEquipMgr::Del(const long long int i64EquipId) {
	/*����װ��ID��ɾ������*/
	if (!m_pUser) {
		cout << "��װ�����������û�ʧ����" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();

	unique_ptr<CEquip> upEquip(Get(i64EquipId));
	if (!upEquip) {
		cout << "ɾ��ʧ�ܣ������û������IDΪ��" << i64EquipId << " ��װ��" << endl;
		return false;
	}

	const CEquipType* pEquipType = g_EquipTypeMgr.Get(upEquip->GetEquipType());
	const long long int i64CardId = GetWearCardId(i64EquipId);

	if (i64CardId) {
		cout << "��װ���ѱ��������������¸�װ��" << endl;
		unsigned int unPos = 0;
		if (pEquipType)
			unPos = pEquipType->GetPos();
		CCard* pCard = CardMgr.Get(i64CardId);
		assert(pCard);//��ʱ���ڴ�����ϵ���ǿ�����ʧ�ˣ����������
		if (pCard)
			pCard->Drop(unPos);
	}
	bool ret = upEquip->Delete();
	m_mapEquipCard.erase(i64EquipId);
	m_map.erase(i64EquipId);
	return ret;
}
bool CEquipMgr::DelExist(const long long int i64EquipId) {
	/*ɾ��һ�����е�װ��*/
	return Del(i64EquipId);
}
bool CEquipMgr::DelEquipCardMap(const long long int i64EquipId) {
	/*�����������ɾ������*/
	if (!Get(i64EquipId)) {
		return false;
	}
	const long long int i64CardId=GetWearCardId(i64EquipId);
	if (i64CardId==0) {
		cout << "��װ��δ������" << endl;
		return false;
	}
	m_mapEquipCard.erase(i64EquipId);
	return true;
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

void CEquipMgr::Free(){
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	m_mapEquipCard.clear();
	for (auto& iter : m_map) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_map.clear();
}