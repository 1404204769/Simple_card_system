#include "CardRankTypeMgr.h"
using namespace std;
CCardRankTypeMgr::CCardRankTypeMgr() {
	/*���캯��*/
	Log("������CCardRankTypeMgr���캯��\n");
}
CCardRankTypeMgr::~CCardRankTypeMgr() {
	/*��������*/
	Free();
	Log("������CCardRankTypeMgr��������\n");
}
const CCardRankType* CCardRankTypeMgr::Get(const unsigned int unId) {
	/*���ݿ��ƽ׼���ȡ����*/
	CardRankTypeMapIter iterById = m_mapById.find(unId);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find CCardLevAttrTypeId:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
bool CCardRankTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardRankTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardRankTypeMgr::Init()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from s_card_rank;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardRankTypeMgr::Init()  �����ݿ���ؿ��Ƴ�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CCardRankTypeMgr::Init()  �����ݿ���ؿ��Ƴسɹ�\n");

		;
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CCardRankType>  pCCardRankType(new CCardRankType());
			if (!pCCardRankType) {
				Log("CCardRankTypeMgr::Init()  ���ƽ׼�����ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!pCCardRankType->Init(row)) {
				Log("CCardRankTypeMgr::Init()  ���ƽ׼����ݳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const unsigned int unCardRankId = pCCardRankType->GetId();
			m_mapById.insert({ unCardRankId,pCCardRankType.release() });
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardRankTypeMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardRankTypeMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CCardRankTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardRankTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CCardRankTypeMgr::Debug_PrintAll() {
	/*��ӡ��ʾ��̬����*/
	for (auto& iter : m_mapById) {
		const CCardRankType* pCardRankType = iter.second;
		if (!pCardRankType)
			continue;
		cout << "Id: " << iter.first << "\tCost_card_num:" << pCardRankType->GetCostCardNum() << "\tHp:" << pCardRankType->GetHp() << "\tMp:" 
			<< pCardRankType->GetMp() << "\tAtk:" << pCardRankType->GetAtk() << endl;
	}
	return true;
}
void CCardRankTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto& iter : m_mapById) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapById.clear();
}
