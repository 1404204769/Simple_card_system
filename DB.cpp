#include "DB.h"
using namespace std;
using namespace mysqlpp;

CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	Log("调用了CDB的构造函数\n");
}
CDB::~CDB()
{
	Log("调用了CDB的析构函数\n");
}
bool CDB::InitConnect(const string& strDB, const string& strServer, const string& strUser, const string& strPassword, const int nPort) {
	try {
		/*建立数据库连接*/
		m_strDB = strDB;
		m_strServer = strServer;
		m_strUser = strUser;
		m_strPassword = strPassword;
		m_nPort = nPort;

		if (!m_Conn.set_option(new mysqlpp::SetCharsetNameOption("latin1")))return false;//设置字符编码为uft8
		if (!m_Conn.set_option(new mysqlpp::ReconnectOption(true)))return false;
		if (!m_Conn.connect(m_strDB.c_str(), m_strServer.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_nPort)) {
			Log("CDB::InitConnect()  数据库链接失败\n");
			return false;
		}

		m_pQuery = new Query(m_Conn.query("set names latin1"));
		if (!m_pQuery)return false;
		if (!m_pQuery->exec()) {
			Log("CDB::InitConnect()  设置latin1格式失败\n");
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CDB::InitConnect()  Query error: " +string( er.what() )+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CDB::InitConnect()  Conversion error: "+string( er.what())+ ",tretrieved data size: "+to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size) +"\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CDB::InitConnect()  Error: "+ string(er.what())+ "\n");
		return false;
	}
	return true;
}
mysqlpp::Query* CDB::GetQuery() {
	try {
		if (!*m_pQuery) {
			delete m_pQuery;
			m_pQuery = new Query(m_Conn.query());
		}
		if (!*m_pQuery) {
			Log("调用CDB::getQuery()获取Query失败\n");
			return nullptr;
		}

		Log("调用CDB::getQuery()获取Query成功\n");
		m_pQuery->reset();

		assert(m_pQuery);

		return m_pQuery;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		Log("CDB::getQuery()  Query error: "+string( er.what())+"\n");
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		Log("CDB::getQuery()  Conversion error: "+string( er.what())+",tretrieved data size: "+to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size) + "\n");
	}
	catch (const mysqlpp::Exception& er) {
		Log( "CDB::getQuery()  Error: "+string( er.what())+ "\n");
	}
	return nullptr;
}
bool CDB::Insert(mysqlpp::Query&query) {
	/*插入一条新的值到数据库中*/
	try {
		Log("CDB::Insert()\n");
		if (!query) {
			Log("CDB::Insert()  Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Insert()  query执行失败，插入失败\n");
			return false;
		}

		long long int i64Num = query.affected_rows();
		if (i64Num == 0) {
			Log("CDB::Insert()  插入失败，数据库被改变" + to_string(i64Num) + "条\n");
			return false;
		}
		Log("CDB::Insert()  插入成功，数据库被改变" + to_string(i64Num)+"条\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CDB::Insert()  Query error: " +string( er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CDB::Insert()  Conversion error: " +string(er.what())+",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CDB::Insert()  Error: " +string( er.what() )+"\n");
		return false;
	}
	return true;
}
bool CDB::Delete(mysqlpp::Query& query) {
	/*插入一条新的值到数据库中*/
	try {
		Log("CDB::Delete()\n");
		if (!query) {
			Log("CDB::Delete()  Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Delete()  query执行失败，删除失败\n");
			return false;
		}

		long long int i64Num = query.affected_rows();
		if (i64Num == 0) {
			Log("CDB::Delete()  删除失败，数据库被改变" +to_string(i64Num) + "条\n");
			return false;
		}
		Log("CDB::Delete()  删除成功，数据库被改变" +to_string(i64Num) + "条\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CDB::Delete()  Query error: "+string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CDB::Delete()  Conversion error: " +string( er.what() )+",tretrieved data size: "+to_string(er.retrieved) + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CDB::Delete()  Error: " +string( er.what() )+ "\n");
		return false;
	}
	return true;
}
bool CDB::Update(mysqlpp::Query& query) {
	/*将数据库中对应的数据修改*/
	try {
		if (!query) {
			Log("CDB::Update()  Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Update()  query执行失败，更新失败\n");
			return false;
		}

		Log("CDB::Update()  更新成功，数据库被改变" + to_string(query.affected_rows())+"条\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CDB::Update()  Query error: " +string( er.what() )+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CDB::Update()  Conversion error: " +string( er.what() )+",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log( "CDB::Update()  Error: " +string( er.what())+ "\n");
		return false;
	}
	return true;
}
bool CDB::Search(UseQueryResult& res, mysqlpp::Query& query) {
	/*查询d_user表内的指定信息*/
	try {
		/*根据输入的数据库项和对应的值查询是否存在*/
		if (!query) {
			Log("CDB::Search()  Query失效，请重新连接\n");
			return false;
		}
		res = query.use();
		if (!res) {
			Log("CDB::Search()  Res失效，请重试\n");
			return false;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CDB::Search()  Query error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CDB::Search()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CDB::Search()  Error: " +string( er.what() )+ "\n");
		return false;
	}
	return true;
}