#include "Equip.h"
using namespace std;
CEquip::CEquip() {
	/*构造函数*/
	Log("调用了CEquip构造函数\n");
}
CEquip::~CEquip() {
	/*析构函数*/
	Log("调用了CEquip析构函数\n");
	Update();
}

bool CEquip::CreateNewEquip(const long long int i64UserId, const CEquipType* pEquipType) {
	/*构造一个新的装备*/
	if (!pEquipType)
	{
		cout << "装备类型无效，新建装备失败" << endl;
		return false;
	}

	if (i64UserId <= 0) {
		cout << "用户ID不合法" << endl;
		return false;
	}

	m_i64UserId = i64UserId;
	m_unEquipType = pEquipType->GetId();

	if (!Insert()) {
		cout << "新装备插入数据库失败" << endl;
		return false;
	}
	return true;
}
bool CEquip::CreateFromDB(const mysqlpp::Row& row, const CEquipType* pEquipType) {
	/*从数据库中加载装备*/
	try
	{
		if (!row)
		{
			Log("初始化装备数据失败\n");//打印在控制台
			return false;
		}/*否则说明找到对应数据，将其映射到CEquip对象上*/

		if (!pEquipType)
		{
			cout << "装备类型无效，新建装备实例失败" << endl;
			return false;
		}

		m_unEquipType = row["equip_type"];
		if (m_unEquipType != pEquipType->GetId())
		{
			cout << "卡牌类型不匹配，新建卡牌失败" << endl;
			return false;
		}
		m_i64UserId = row["user_id"];
		m_i64EquipId = row["id"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::CreateFromDB()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquip::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
unsigned int CEquip::GetEquipType() const {
	/*获取m_unEquipType*/
	return m_unEquipType;
}
long long int CEquip::GetEquipId() const {
	/*获取m_i64EquipId*/
	return m_i64EquipId;
}
long long int CEquip::GetUserId() const {
	/*获取m_i64UserId*/
	return m_i64UserId;
}
bool CEquip::Insert() {
	/*将Card数据插入数据库*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CEquip::Insert()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "insert into d_skin values(0,%0q:user_id,%1q:equip_type)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["equip_type"] = m_unEquipType;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CEquip::Insert()  往数据库插入用户新装备数据失败\n");
			return false;
		}
		Log("CEquip::Insert()  往数据库插入用户新装备数据成功\n");

		m_i64EquipId = pQuery->insert_id();
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::Insert()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::Insert()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquip::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CEquip::Delete() {
	/*将Card数据从数据库删除*/
	try
	{
		Log("CEquip::Delete()\n");
		if (m_i64EquipId == 0) {
			Log("CEquip::Delete()  装备Id失效\n");
			return false;
		}

		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CEquip::Delete()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "delete from d_equip where id = %0q:EquipId;";
		pQuery->parse();
		pQuery->template_defaults["EquipId"] = m_i64EquipId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CEquip::Delete()  从数据库删除用户装备数据失败\n");
			return false;
		}
		Log("CEquip::Delete()  从数据库删除用户装备数据成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::Delete()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::Delete()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
//bool CEquip::Update() {
//	/*将最新的数据更新到数据库*/
//	try
//	{
//		Log("CEquip::Update()\n");
//		mysqlpp::Query* pQuery = g_DB.GetQuery();
//		if (!*pQuery) {
//			Log("CEquip::Update()  Query对象不存在，无法更新数据\n");
//			return false;
//		}
//
//		*pQuery << "update `d_equip` set user_id=%0q:UserId,equip_type=%1q:EquipType where id=%2q:id;";
//		pQuery->parse();
//		pQuery->template_defaults["UserId"] = m_i64UserId;
//		pQuery->template_defaults["EquipType"] = m_unEquipType;
//		pQuery->template_defaults["id"] = m_i64EquipId;
//
//		Log("Query:" + pQuery->str() + "\n");
//		if (!g_DB.Update(*pQuery)) {
//			Log("CEquip::Update()向数据库更新装备失败\n");
//			return false;
//		}
//		Log("CEquip::Update()向数据库更新装备成功\n");
//	}
//	catch (const mysqlpp::BadQuery& er) {
//		Log("CEquip::Update()  Query error: " + string(er.what()) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::BadConversion& er) {
//		Log("CEquip::Update()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::BadIndex& er) {
//		Log("CEquip::Update()  Error: " + string(er.what()) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::Exception& er) {
//		Log("CEquip::Update()  Error: " + string(er.what()) + "\n");
//		return false;
//	}
//	return true;
//}