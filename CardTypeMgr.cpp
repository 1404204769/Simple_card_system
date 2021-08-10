#include "CardTypeMgr.h"
using namespace std;
CCardTypeMgr::CCardTypeMgr() {
	/*���캯��*/
	stringstream strIn;
	strIn << "������CCardTypeMgr���캯��\n";
	OutputDebugPrintf(strIn.str().c_str());
}
CCardTypeMgr::~CCardTypeMgr() {
	/*��������*/
	stringstream strIn;
	strIn << "������CCardTypeMgr��������\n";
	OutputDebugPrintf(strIn.str().c_str());
}
const CCardType* CCardTypeMgr::GetCardTypeByType(unsigned int _unType) {
	/*���ݿ���ID��ȡ����*/
	/*��map�����ѯ�����ز�ѯ����ָ��*/
    MapByTypeIter iterByType= m_mapByType.find(_unType);
	if (iterByType == m_mapByType.end()) {
		cout << "Do not Find CardTypeID:" << _unType << endl;
		return nullptr;
	}
	return iterByType->second;
}
void CCardTypeMgr::PrintAllCardType() {
	/*��ӡ��ʾ���п�������*/
	MapByTypeIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		const CCardType* pCardType = iterByType->second;
		iterByType++;
		if (!pCardType) {
			continue;
		}
		cout << "ID:" << pCardType->getId() << "\tCardType:" << pCardType->getCardType() << "\tName:" << pCardType->getName()<<"\tHp:"<< pCardType->getHp()<< "\tMp:" << pCardType->getMp() << "\tAtk:" << pCardType->getAtk() << endl;
	}
}
bool CCardTypeMgr::Init() {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardType�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.getQuery();
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\n";
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "select * from s_card;";
		pQuery->parse();
		bool bRet = g_DB.Search(res, *pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			strIn << "�����ݿ���ؿ��Ƴ�ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
			return false;
		}
		strIn << "�����ݿ���ؿ��Ƴسɹ�\n";
		mysqlpp::Row row;
		while (row = res.fetch_row()) {
			CCardType* pCardType=new CCardType();
			if (!pCardType) {
				strIn << "��������ʵ�廯ʧ��\n";
				OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
				delete pCardType;
				pCardType = nullptr;
				return false;
			}
			if (!pCardType->Init(row)) {
				strIn << "�������ͳ�ʼ��ʧ��\n";
				OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
				delete pCardType;
				pCardType = nullptr;
				return false;
			}
			m_mapByType[pCardType->getCardType()] = pCardType;
		}
		OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}
void CCardTypeMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	MapByTypeIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		delete iterByType->second;//���ͷ��ڴ�
		iterByType->second = nullptr;//�ÿ�
		iterByType++;
	}
	m_mapByType.clear();
}
