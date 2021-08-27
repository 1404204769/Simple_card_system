#include "CardLevAttrTypeMgr.h"
using namespace std;
CCardLevAttrTypeMgr::CCardLevAttrTypeMgr() {
	/*构造函数*/
	Log("调用了CCardLevAttrTypeMgr构造函数\n");
}
CCardLevAttrTypeMgr::~CCardLevAttrTypeMgr() {
	/*析构函数*/
	Log("调用了CCardLevAttrTypeMgr析构函数\n");
}
const CCardLevAttrType* CCardLevAttrTypeMgr::Get(unsigned int unLev) {
	/*根据卡牌Lev获取数据*/
	/*从map里面查询，返回查询对象指针*/
	CardLevAttrTypeMapIter iterByLev = m_mapByLev.find(unLev);
	if (iterByLev == m_mapByLev.end()) {
		cout << "Do not Find CCardLevAttrTypeLev:" << unLev << endl;
		return nullptr;
	}
	assert(iterByLev->second);

	return iterByLev->second;
}
bool CCardLevAttrTypeMgr::Debug_PrintAll() {
	/*打印显示静态配置*/
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
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardLevAttrTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardLevAttrTypeMgr::Init()  Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from s_card_lev_attr;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardLevAttrTypeMgr::Init()  从数据库加载卡牌池失败\n");//打印在控制台
			return false;
		}
		Log("CCardLevAttrTypeMgr::Init()  从数据库加载卡牌池成功\n");

		;
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CCardLevAttrType>  pCardLevAttrType(new CCardLevAttrType());
			if (!pCardLevAttrType) {
				Log("CCardLevAttrTypeMgr::Init()  卡牌类型实体化失败\n");//打印在控制台
				return false;
			}
			if (!pCardLevAttrType->Init(row)) {
				Log("CCardLevAttrTypeMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
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
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for (auto& iter:m_mapByLev) {
		delete iter.second;//先释放内存
	}
	m_mapByLev.clear();
}
