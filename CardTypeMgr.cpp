#include "CardTypeMgr.h"
using namespace std;
CCardTypeMgr::CCardTypeMgr() {
	/*构造函数*/
	Log("调用了CCardTypeMgr构造函数\n");
}
CCardTypeMgr::~CCardTypeMgr() {
	/*析构函数*/
	Free();
	Log("调用了CCardTypeMgr析构函数\n");
}
const CCardType* CCardTypeMgr::Get(unsigned int _unType) {
	/*根据卡牌ID获取数据*/
	/*从map里面查询，返回查询对象指针*/
	CardTypeMapIter iterByType = m_mapByType.find(_unType);
	if (iterByType == m_mapByType.end()) {
		cout << "Do not Find CardTypeID:" << _unType << endl;
		return nullptr;
	}
	return iterByType->second;
}
void CCardTypeMgr::PrintAll() {
	/*打印显示所有卡牌类型*/
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
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardTypeMgr::Init()  Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from s_card;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardTypeMgr::Init()  从数据库加载卡牌池失败\n");//打印在控制台
			return false;
		}
		Log("CCardTypeMgr::Init()  从数据库加载卡牌池成功\n");

		mysqlpp::Row row;
		while (row = res.fetch_row()) {
			unique_ptr<CCardType>  pCardType(new CCardType());
			if (!pCardType) {
				Log("CCardTypeMgr::Init()  卡牌类型实体化失败\n");//打印在控制台
				return false;
			}
			if (!pCardType->Init(row)) {
				Log("CCardTypeMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
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
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	CardTypeMapIter iterByType = m_mapByType.begin();
	while (iterByType != m_mapByType.end()) {
		delete iterByType->second;//先释放内存
		iterByType->second = nullptr;//置空
		iterByType++;
	}
	m_mapByType.clear();
}
