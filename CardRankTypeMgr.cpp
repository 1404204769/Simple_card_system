#include "CardRankTypeMgr.h"
using namespace std;
CCardRankTypeMgr::CCardRankTypeMgr() {
	/*构造函数*/
	Log("调用了CCardRankTypeMgr构造函数\n");
}
CCardRankTypeMgr::~CCardRankTypeMgr() {
	/*析构函数*/
	Free();
	Log("调用了CCardRankTypeMgr析构函数\n");
}
const CCardRankType* CCardRankTypeMgr::Get(const unsigned int unId) {
	/*根据卡牌阶级获取数据*/
	CardRankTypeMapIter iterById = m_mapById.find(unId);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find CCardLevAttrTypeId:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
bool CCardRankTypeMgr::Init() {
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CCardRankTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CCardRankTypeMgr::Init()  Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from s_card_rank;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardRankTypeMgr::Init()  从数据库加载卡牌池失败\n");//打印在控制台
			return false;
		}
		Log("CCardRankTypeMgr::Init()  从数据库加载卡牌池成功\n");

		;
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CCardRankType>  pCCardRankType(new CCardRankType());
			if (!pCCardRankType) {
				Log("CCardRankTypeMgr::Init()  卡牌阶级数据实体化失败\n");//打印在控制台
				return false;
			}
			if (!pCCardRankType->Init(row)) {
				Log("CCardRankTypeMgr::Init()  卡牌阶级数据初始化失败\n");//打印在控制台
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
	/*打印显示静态配置*/
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
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for (auto& iter : m_mapById) {
		delete iter.second;//先释放内存
	}
	m_mapById.clear();
}
