#include "SkinTypeMgr.h"
using namespace std;
CSkinTypeMgr::CSkinTypeMgr() {
	/*构造函数*/
	Log("调用了CSkinTypeMgr构造函数\n");
}
CSkinTypeMgr::~CSkinTypeMgr() {
	/*析构函数*/
	Log("调用了CSkinTypeMgr析构函数\n");
}
const CSkinType* CSkinTypeMgr::Get(unsigned int unId) {
	/*根据卡牌ID获取数据*/
	/*从map里面查询，返回查询对象指针*/
	SkinTypeMapIter iterById = m_mapSkinType.find(unId);
	if (iterById == m_mapSkinType.end()) {
		cout << "Do not Find SkinTypeID:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
void CSkinTypeMgr::PrintAll() {
	/*打印显示所有卡牌类型*/
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
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CSkinTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CSkinTypeMgr::Init()  Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from s_skin;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CSkinTypeMgr::Init()  从数据库加载卡牌池失败\n");//打印在控制台
			return false;
		}
		Log("CSkinTypeMgr::Init()  从数据库加载卡牌池成功\n");

		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CSkinType>  pSkinType(new CSkinType());
			if (!pSkinType) {
				Log("CSkinTypeMgr::Init()  卡牌类型实体化失败\n");//打印在控制台
				return false;
			}
			if (!pSkinType->Init(row)) {
				Log("CSkinTypeMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
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
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for(auto &iter:m_mapSkinType) {
		delete iter.second;//先释放内存
	}
	m_mapSkinType.clear();
}
