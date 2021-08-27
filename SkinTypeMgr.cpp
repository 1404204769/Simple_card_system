#include "SkinTypeMgr.h"
using namespace std;
CSkinTypeMgr::CSkinTypeMgr() {
	/*���캯��*/
	Log("������CSkinTypeMgr���캯��\n");
}
CSkinTypeMgr::~CSkinTypeMgr() {
	/*��������*/
	Log("������CSkinTypeMgr��������\n");
}
const CSkinType* CSkinTypeMgr::Get(unsigned int unId) {
	/*���ݿ���ID��ȡ����*/
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	SkinTypeMapIter iterById = m_mapSkinType.find(unId);
	if (iterById == m_mapSkinType.end()) {
		cout << "Do not Find SkinTypeID:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
void CSkinTypeMgr::PrintAll() {
	/*��ӡ��ʾ���п�������*/
	for(auto &iter:m_mapSkinType) {
		const CSkinType* pSkinType = iter.second;
		if (!pSkinType) {
			continue;
		}
		cout << "ID:" << pSkinType->GetId() << "\tName:" << pSkinType->GetName()
			<< "\tHp:" << pSkinType->GetHp() << "\tMp:" << pSkinType->GetMp() << "\tAtk:" << pSkinType->GetAtk() << endl;
	}
}
bool CSkinTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CSkinTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CSkinTypeMgr::Init()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from s_skin;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CSkinTypeMgr::Init()  �����ݿ���ؿ��Ƴ�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CSkinTypeMgr::Init()  �����ݿ���ؿ��Ƴسɹ�\n");

		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CSkinType>  pSkinType(new CSkinType());
			if (!pSkinType) {
				Log("CSkinTypeMgr::Init()  ��������ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!pSkinType->Init(row)) {
				Log("CSkinTypeMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const unsigned int unSkinType = pSkinType->GetId();
			m_mapSkinType.insert({ unSkinType ,pSkinType.release() });
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkinTypeMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkinTypeMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CSkinTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkinTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
void CSkinTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for(auto &iter:m_mapSkinType) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapSkinType.clear();
}
