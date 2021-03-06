#include "Skin.h"
#include "Card.h"
using namespace std;
CSkin::CSkin() {
	/*构造函数*/
	Log("调用了CSkin构造函数\n");
}
CSkin::~CSkin() {
	/*析构函数*/
	Log("调用了CSkin析构函数\n");
	Update();
}

bool CSkin::CreateNewSkin(const long long int i64UserId, const CSkinType* pSkinType) {
	/*构造一个新的皮肤*/
	if (!pSkinType)
	{
		cout << "皮肤类型无效，新建卡牌失败" << endl;
		return false;
	}

	if (i64UserId <= 0) {
		cout << "用户ID不合法" << endl;
		return false;
	}

	m_i64UserId = i64UserId;
	m_unSkinType = pSkinType->GetId();
	m_i64CardId = 0;

	if (!Insert()) {
		cout << "新皮肤插入数据库失败" << endl;
		return false;
	}
	return true;
}
bool CSkin::CreateFromDB(const mysqlpp::Row& row, const CSkinType* pSkinType) {
	/*从数据库中加载皮肤数据*/
	try
	{
		if (!row)
		{
			Log("初始化皮肤数据失败\n");//打印在控制台
			return false;
		}/*否则说明找到对应数据，将其映射到CSkin对象上*/

		if (!pSkinType)
		{
			cout << "皮肤类型无效，新建皮肤实例失败" << endl;
			return false;
		}

		m_unSkinType = row["skin_type"];
		if (m_unSkinType != pSkinType->GetId())
		{
			cout << "卡牌类型不匹配，新建卡牌失败" << endl;
			return false;
		}

		m_i64CardId = row["card_id"];
		m_i64UserId = row["user_id"];
		m_i64SkinId = row["id"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::CreateFromDB()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

unsigned int CSkin::GetSkinType() const {
	/*获取m_unSkindType*/
	return m_unSkinType;
}
long long int CSkin::GetCardId() const {
	/*获取m_i64CardId*/
	return m_i64CardId;
}
long long int CSkin::GetSkinId() const {
	/*获取m_i64SkinId*/
	return m_i64SkinId;
}
long long int CSkin::GetUserId() const {
	/*获取m_i64UserId*/
	return m_i64UserId;
}
bool CSkin::IsWear()const {
	if (GetCardId() == 0)/*此时还没有卡牌穿戴这个皮肤*/
		return false;
	return true;
}

bool CSkin::Wear(CCard& Card) {
	/*穿上某个皮肤*/
	if (IsWear()) {//如果还没被穿
		cout << "皮肤ID:" << m_i64SkinId << "已经被别的卡牌(ID: " << m_i64CardId << ")穿上" << endl;
		return false;
	}
	m_i64CardId = Card.GetCardId();
	return true;
}
bool CSkin::Drop(CCard& Card) {
	/*脱下某个皮肤*/
	if (!IsWear()) {//还没被穿
		cout << "皮肤ID:"<< m_i64SkinId <<"还没被穿上" << endl;
		return false;
	}
	if (m_i64CardId != Card.GetCardId()) {
		cout << "皮肤ID:" << m_i64SkinId << " 穿在卡牌ID："<< m_i64CardId <<"  其余卡牌不得对其操作" << endl;
		return false;
	}
	m_i64CardId = 0;
	return true;
}

/*
* 数据库相关操作
*/

bool CSkin::Delete() {
	/*将Skin数据从数据库删除*/
	try
	{
		Log("CSkin::Delete()\n");
		if (m_i64SkinId == 0) {
			Log("CSkin::Delete()  皮肤Id失效\n");
			return false;
		}

		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CSkin::Delete()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "delete from d_skin where id = %0q:SkinId;";
		pQuery->parse();
		pQuery->template_defaults["SkinId"] = m_i64SkinId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CSkin::Delete()  从数据库删除用户皮肤数据失败\n");
			return false;
		}
		Log("CSkin::Delete()  从数据库删除用户皮肤数据成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::Delete()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::Delete()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CSkin::Insert() {
	/*将Skin数据插入数据库*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CSkin::Insert()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "insert into d_skin values(0,%0q:user_id, %1q:card_id, %2q:skin_type)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["card_id"] = m_i64CardId;
		pQuery->template_defaults["skin_type"] = m_unSkinType;
		
		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CSkin::Insert()  往数据库插入用户新皮肤数据失败\n");
			return false;
		}
		Log("CSkin::Insert()  往数据库插入用户新皮肤数据成功\n");

		m_i64SkinId = pQuery->insert_id();
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::Insert()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::Insert()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CSkin::Update() {
	/*将最新的数据更新到数据库*/
	try
	{
		Log("CSkin::Update()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CSkin::Update()  Query对象不存在，无法更新数据\n");
			return false;
		}

		*pQuery << "update `d_skin` set user_id=%0q:UserId,card_id=%1q:CardId,skin_type=%2q:SkinType where id=%3q:id;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_i64UserId;
		pQuery->template_defaults["CardId"] = m_i64CardId;
		pQuery->template_defaults["SkinType"] = m_unSkinType;
		pQuery->template_defaults["id"] = m_i64SkinId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Update(*pQuery)) {
			Log("CSkin::Update()向数据库更新皮肤失败\n");
			return false;
		}
		Log("CSkin::Update()向数据库更新皮肤成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::Update()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::Update()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::Update()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Update()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}