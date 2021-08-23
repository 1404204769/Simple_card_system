#include "EquipTypeMgr.h"
using namespace std;
CEquipTypeMgr::CEquipTypeMgr() {
	/*���캯��*/
	Log("������CEquipTypeMgr���캯��\n");
}
CEquipTypeMgr::~CEquipTypeMgr() {
	/*��������*/
	Free();
	Log("������CEquipTypeMgr��������\n");
}

const CEquipType* CEquipTypeMgr::Get(const unsigned int unId) {
	/*����װ�����ͻ�ȡ����*/
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	EquipTypeMapIter iterById = m_mapEquipType.find(unId);
	if (iterById == m_mapEquipType.end()) {
		cout << "Do not Find EquipTypeID:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}

void CEquipTypeMgr::PrintAll() {
	/*��ӡ��ʾ����װ������*/
	for (auto& iter : m_mapEquipType) {
		const CEquipType* pEquipType = iter.second;
		if (!pEquipType) {
			continue;
		}
		cout << "ID:" << pEquipType->GetId() << "\tName:" << pEquipType->GetName()<<"\tPos:"<<pEquipType->GetPos()
			<< "\tHp:" << pEquipType->GetHp() << "\tMp:" << pEquipType->GetMp() << "\tAtk:" << pEquipType->GetAtk() << endl;
	}
}
bool CEquipTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CEquipTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CEquipTypeMgr::Init()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from s_equip;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CEquipTypeMgr::Init()  �����ݿ����װ�����ó�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CEquipTypeMgr::Init()  �����ݿ����װ�����óɹ�\n");

		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CEquipType>  pEquipType(new CEquipType());
			if (!pEquipType) {
				Log("CEquipTypeMgr::Init()  װ������ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!pEquipType->Init(row)) {
				Log("CEquipTypeMgr::Init()  װ�����ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const unsigned int unEquipType = pEquipType->GetId();
			m_mapEquipType.insert({ unEquipType ,pEquipType.release() });
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquipTypeMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquipTypeMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CEquipTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquipTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
void CEquipTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto& iter : m_mapEquipType) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapEquipType.clear();
}
