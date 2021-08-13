#include "User.h"
#include "CardMgr.h"
using namespace std;
CUser::CUser(const std::string& _strAccount) {
	m_strAccount = _strAccount;
	Log("调用了CUser构造函数\n");
}

CUser::~CUser()
{
	Log( "调用了CUser的析构函数\n");

	if (!Update(*this))
		Log("用户数据保存失败\n");
	else
		Log("用户数据保存成功\n");
	delete m_pCardMgr;
	m_pCardMgr = nullptr;
}


const std::string& CUser::GetAccount() const{
	return m_strAccount;
}
const std::string& CUser::GetCreateTime() const{
	return m_strCreateTime;
}
const std::string& CUser::GetName() const{
	return m_strName;
}
long long int CUser::GetId() const{
	return m_i64Id;
}
long long int CUser::GetExp() const{
	return m_i64Exp;
}
unsigned int CUser::GetLev() const{
	return m_unLev;
}
CCardMgr& CUser::GetCardMgr() const{
	assert(m_pCardMgr);

	return *m_pCardMgr;
}
void CUser::SetLev(const unsigned int _unLev) {
	m_unLev = _unLev;
}
void CUser::SetName(const string& _strName) {
	m_strName = _strName;
}
void CUser::SetExp(const long long int _i64Exp) {
	m_i64Exp = _i64Exp;
}

/*
* 以下是数据库层相关接口
*/
bool CUser::Insert() {
	/*在数据库中插入新的User数据*/
	try
	{
		Log( "CUser::InsertUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::Insert() Query实例指针错误\n");
			return false;
		}

		*pQuery << "insert into d_user(account,name) values(%0q:account, %1q:name)";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();
		pQuery->template_defaults["name"] = m_strName.c_str();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Insert(*pQuery)) {
			Log("CUser::Insert() 往数据库插入新用户失败\n");
			return false;
		}
		Log("CUser::Insert() 往数据库插入新用户成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CUser::InsertUser()  Query error: " +string( er.what() )+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CUser::InsertUser()  Conversion error: " +string( er.what()) + ",tretrieved data size: " +to_string(er.retrieved) + ", actual size: " +to_string( er.actual_size )+ "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CUser::InsertUser()  Error: " +string( er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CUser::InsertUser()  Error: " +string( er.what()) + "\n");
		return false;
	}
	return true;
}
bool CUser::Delete() {
	/*在数据库中删除指定User数据*/
	try
	{
		Log("CUser::DeleteUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::Delete() Query实例指针错误\n");
			return false;
		}

		*pQuery << "delete from d_user where account = %0q:account;";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CUser::Delete() 从数据库删除用户失败\n");
			return false;
		}
		Log("CUser::Delete() 从数据库删除用户成功\n");

		if (!m_pCardMgr->DelAll()) {
			Log("CUser::Delete() 从数据库删除用户持有的卡牌失败\n");
			return false;
		}
		Log("CUser::Delete() 从数据库删除用户持有的卡牌成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CUser::DeleteUser()  Query error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CUser::DeleteUser()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CUser::DeleteUser()  Error: " +string(er.what())+  "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CUser::DeleteUser()  Error: " +string( er.what()) + "\n");
		return false;
	}
	return true;
}
bool CUser::Update(CUser& user) {
	/*在数据库中更新指定User*/
	try
	{
		Log("CUser::UpdateUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::UpdateUser()  Query对象不存在，无法更新数据\n");
			return false;
		}

		*pQuery << "update `d_user` set account=%0q:account,`name`=%1q:name,exp=%2:exp,lev=%3:lev where id=%4:id;";
		pQuery->parse();
		pQuery->template_defaults["account"] = user.m_strAccount.c_str();
		pQuery->template_defaults["name"] = user.m_strName.c_str();
		pQuery->template_defaults["exp"] = user.m_i64Exp;
		pQuery->template_defaults["lev"] = user.m_unLev;
		pQuery->template_defaults["id"] = user.m_i64Id;

		Log("Query:"+ pQuery->str()+"\n");
		if (!g_DB.Update(*pQuery)) {
			Log("CUser::UpdateUser()向数据库更新用户失败\n");
			return false;
		}
		Log("CUser::UpdateUser()向数据库更新用户成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CUser::UpdateUser()  Query error: " +string( er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log( "CUser::UpdateUser()  Conversion error: " +string( er.what() )+ ",tretrieved data size: " +to_string(er.retrieved) +", actual size: " +to_string(er.actual_size)  +"\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CUser::UpdateUser()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log( "CUser::UpdateUser()  Error: " +string( er.what())+ "\n");
		return false;
	}
	return true;

}

bool CUser::Init(const mysqlpp::Row& row) {
	/*根据查询获取的mysqlpp::Row对User对象初始化*/
	try
	{
		if (!row) {
			Log("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/

		m_strAccount = row["account"];
		m_strCreateTime = row["create_time"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_unLev = row["lev"];
		m_i64Id = row["id"];
		m_pCardMgr = new CCardMgr();

		if (!m_pCardMgr->Init(this)) {
			cout << "用户拥有的卡牌加载失败" << endl;
			delete m_pCardMgr;
			m_pCardMgr = nullptr;
			return false;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CUser::InitUser()  Query error: " +string( er.what() )+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CUser::InitUser()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CUser::InitUser()  Error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CUser::InitUser()  Error: " +string( er.what()) + "\n");
		return false;
	}
	return true;
}