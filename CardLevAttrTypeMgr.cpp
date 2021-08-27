#include "CardLevAttrTypeMgr.h"
using namespace std;
CCardLevAttrTypeMgr::CCardLevAttrTypeMgr() {
	/*���캯��*/
	Log("������CCardLevAttrTypeMgr���캯��\n");
}
CCardLevAttrTypeMgr::~CCardLevAttrTypeMgr() {
	/*��������*/
	Log("������CCardLevAttrTypeMgr��������\n");
}
const CCardLevAttrType* CCardLevAttrTypeMgr::Get(unsigned int unLev) {
	/*���ݿ���Lev��ȡ����*/
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	CardLevAttrTypeMapIter iterByLev = m_mapByLev.find(unLev);
	if (iterByLev == m_mapByLev.end()) {
		cout << "Do not Find CCardLevAttrTypeLev:" << unLev << endl;
		return nullptr;
	}
	assert(iterByLev->second);

	return iterByLev->second;
}
bool CCardLevAttrTypeMgr::Debug_PrintAll() {
	/*��ӡ��ʾ��̬����*/
	for (auto& iter : m_mapByLev) {
		const CCardLevAttrType* pCardLevAttrType = iter.second;
		if (!pCardLevAttrType)
			continue;
		cout << "Id:" << iter.first << "\tLev:" << pCardLevAttrType->GetCardLev() << "\tHp:" << pCardLevAttrType->GetHp() 
			<< "\tMp:"<< pCardLevAttrType->GetMp() << "\tAtk:" << pCardLevAttrType->GetAtk() << endl;
	}
	return true;
}
bool CCardLevAttrTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardLevAttrTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardLevAttrTypeMgr::Init()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from s_card_lev_attr;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardLevAttrTypeMgr::Init()  �����ݿ���ؿ��Ƴ�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CCardLevAttrTypeMgr::Init()  �����ݿ���ؿ��Ƴسɹ�\n");

		;
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CCardLevAttrType>  pCardLevAttrType(new CCardLevAttrType());
			if (!pCardLevAttrType) {
				Log("CCardLevAttrTypeMgr::Init()  ��������ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!pCardLevAttrType->Init(row)) {
				Log("CCardLevAttrTypeMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const unsigned int unCardLev = pCardLevAttrType->GetCardLev();
			m_mapByLev.insert({ unCardLev,pCardLevAttrType.release()});
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardLevAttrTypeMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardLevAttrTypeMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CCardLevAttrTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardLevAttrTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
void CCardLevAttrTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto& iter:m_mapByLev) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapByLev.clear();
}
