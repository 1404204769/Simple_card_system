#include "CardTypeMgr.h"
using namespace std;
CCardTypeMgr::CCardTypeMgr() {
	/*���캯��*/
	Log("������CCardTypeMgr���캯��\n");
}
CCardTypeMgr::~CCardTypeMgr() {
	/*��������*/
	Free();
	Log("������CCardTypeMgr��������\n");
}
const CCardType* CCardTypeMgr::Get(unsigned int _unType) {
	/*���ݿ���ID��ȡ����*/
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	CardTypeMapIter iterByType = m_mapByType.find(_unType);
	if (iterByType == m_mapByType.end()) {
		cout << "Do not Find CardTypeID:" << _unType << endl;
		return nullptr;
	}
	return iterByType->second;
}
void CCardTypeMgr::PrintAll() {
	/*��ӡ��ʾ���п�������*/
	CardTypeMapIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		const CCardType* pCardType = iterByType->second;
		iterByType++;
		if (!pCardType) {
			continue;
		}
		cout << "ID:" << pCardType->GetId() << "\tCardType:" << pCardType->GetCardType() << "\tName:" << pCardType->GetName() << "\tHp:" << pCardType->GetHp() << "\tMp:" << pCardType->GetMp() << "\tAtk:" << pCardType->GetAtk() << endl;
	}
}
bool CCardTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardTypeMgr::Init()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from s_card;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardTypeMgr::Init()  �����ݿ���ؿ��Ƴ�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CCardTypeMgr::Init()  �����ݿ���ؿ��Ƴسɹ�\n");

		mysqlpp::Row row;
		while (row = res.fetch_row()) {
			unique_ptr<CCardType>  pCardType(new CCardType());
			if (!pCardType) {
				Log("CCardTypeMgr::Init()  ��������ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!pCardType->Init(row)) {
				Log("CCardTypeMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			m_mapByType[pCardType->GetCardType()] = pCardType.release();
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardTypeMgr::Init()  Query error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardTypeMgr::Init()  Conversion error: " +string(er.what())+ ",tretrieved data size: " +to_string(er.retrieved ) + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CCardTypeMgr::Init()  Error: " +string( er.what() )+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
void CCardTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	CardTypeMapIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		delete iterByType->second;//���ͷ��ڴ�
		iterByType->second = nullptr;//�ÿ�
		iterByType++;
	}
	m_mapByType.clear();
}
