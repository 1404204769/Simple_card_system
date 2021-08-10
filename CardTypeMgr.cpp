#include "CardTypeMgr.h"
using namespace std;
CCardTypeMgr::CCardTypeMgr() {
	/*构造函数*/
	stringstream strIn;
	strIn << "调用了CCardTypeMgr构造函数\n";
	OutputDebugPrintf(strIn.str().c_str());
}
CCardTypeMgr::~CCardTypeMgr() {
	/*析构函数*/
	stringstream strIn;
	strIn << "调用了CCardTypeMgr析构函数\n";
	OutputDebugPrintf(strIn.str().c_str());
}
const CCardType* CCardTypeMgr::GetCardTypeByType(unsigned int _unType) {
	/*根据卡牌ID获取数据*/
	/*从map里面查询，返回查询对象指针*/
    MapByTypeIter iterByType= m_mapByType.find(_unType);
	if (iterByType == m_mapByType.end()) {
		cout << "Do not Find CardTypeID:" << _unType << endl;
		return nullptr;
	}
	return iterByType->second;
}
void CCardTypeMgr::PrintAllCardType() {
	/*打印显示所有卡牌类型*/
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
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.getQuery();
		stringstream strIn;
		strIn << "CCardTypeMgr::Init()\n";
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
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
			strIn << "从数据库加载卡牌池失败\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
			return false;
		}
		strIn << "从数据库加载卡牌池成功\n";
		mysqlpp::Row row;
		while (row = res.fetch_row()) {
			CCardType* pCardType=new CCardType();
			if (!pCardType) {
				strIn << "卡牌类型实体化失败\n";
				OutputDebugPrintf(strIn.str().c_str());//打印在控制台
				delete pCardType;
				pCardType = nullptr;
				return false;
			}
			if (!pCardType->Init(row)) {
				strIn << "卡牌类型初始化失败\n";
				OutputDebugPrintf(strIn.str().c_str());//打印在控制台
				delete pCardType;
				pCardType = nullptr;
				return false;
			}
			m_mapByType[pCardType->getCardType()] = pCardType;
		}
		OutputDebugPrintf(strIn.str().c_str());//打印在控制台
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
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	MapByTypeIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		delete iterByType->second;//先释放内存
		iterByType->second = nullptr;//置空
		iterByType++;
	}
	m_mapByType.clear();
}
