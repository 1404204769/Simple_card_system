#include "EquipTypeMgr.h"
using namespace std;
CEquipTypeMgr::CEquipTypeMgr() {
	/*构造函数*/
	Log("调用了CEquipTypeMgr构造函数\n");
}
CEquipTypeMgr::~CEquipTypeMgr() {
	/*析构函数*/
	Free();
	Log("调用了CEquipTypeMgr析构函数\n");
}

const CEquipType* CEquipTypeMgr::Get(const unsigned int unId) {
	/*根据装备类型获取数据*/
	/*从map里面查询，返回查询对象指针*/
	EquipTypeMapIter iterById = m_mapEquipType.find(unId);
	if (iterById == m_mapEquipType.end()) {
		cout << "Do not Find EquipTypeID:" << unId << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}

void CEquipTypeMgr::PrintAll() {
	/*打印显示所有装备类型*/
	for (auto& iter : m_mapEquipType) {
		const CEquipType* pEquipType = iter.second;
		if (!pEquipType) {
			continue;
		}
		cout << "ID:" << pEquipType->GetId() << "\tName:" << pEquipType->GetName()<<"\tPos:"<<pEquipType->GetPos()
			<< "\tHp:" << pEquipType->GetHp() << "\tMp:" << pEquipType->GetMp() << "\tAtk:" << pEquipType->GetAtk() << endl;
	}
}
bool CEquipTypeMgr::Init() {
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CCardType对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CEquipTypeMgr::Init()\n");
		if (!*pQuery) {
			Log("CEquipTypeMgr::Init()  Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from s_equip;";
		pQuery->parse();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CEquipTypeMgr::Init()  从数据库加载装备配置池失败\n");//打印在控制台
			return false;
		}
		Log("CEquipTypeMgr::Init()  从数据库加载装备配置成功\n");

		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CEquipType>  pEquipType(new CEquipType());
			if (!pEquipType) {
				Log("CEquipTypeMgr::Init()  装备类型实体化失败\n");//打印在控制台
				return false;
			}
			if (!pEquipType->Init(row)) {
				Log("CEquipTypeMgr::Init()  装备类型初始化失败\n");//打印在控制台
				return false;
			}
			const unsigned int unEquipType = pEquipType->GetId();
			m_mapEquipType.insert({ unEquipType ,pEquipType.release() });
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquipTypeMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquipTypeMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er)
	{
		Log("CEquipTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquipTypeMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
void CEquipTypeMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for (auto& iter : m_mapEquipType) {
		delete iter.second;//先释放内存
	}
	m_mapEquipType.clear();
}
