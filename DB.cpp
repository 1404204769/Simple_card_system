#include "DB.h"
using namespace std;
using namespace mysqlpp;
CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	cout << "调用了CDB的构造函数" << endl;
}
CDB::~CDB()
{
	cout << "调用了CDB的析构函数" << endl;
	m_Conn.~Connection();
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
			//cout << m_Conn.error() << endl;
			cout << "数据库链接失败" << endl;
			return false;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
}
bool CDB::getQuery(mysqlpp::Query& query) {
	try {
		query = m_Conn.query();
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
	return true;
}
bool CDB::Insert(mysqlpp::Query&query) {
	/*插入一条新的值到数据库中*/
	try {
		if (!query) {
			cerr << "Query失效，请重新连接" << endl;
			return false;
		}
		if (!query.exec()) {
			cerr << "query执行失败，插入失败" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "插入失败，数据库被改变" << nNum << "条" << endl;
			return false;
		}
		cout << "插入成功，数据库被改变" << nNum << "条" << endl;
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
}
bool CDB::Delete(mysqlpp::Query& query) {
	/*插入一条新的值到数据库中*/
	try {
		if (!query) {
			cerr << "Query失效，请重新连接" << endl;
			return false;
		}
		if (!query.exec()) {
			cerr << "query执行失败，删除失败" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "删除失败，数据库被改变"<< nNum<<"条" << endl;
			return false;
		}
		cout << "删除成功，数据库被改变" << nNum << "条" << endl;
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
}
bool CDB::Update(mysqlpp::Query& query) {
	/*将数据库中对应的数据修改*/
	try {
		if (!query) {
			cout << "Query失效，请重新连接" << endl;
			return false;
		}
		//cout << "Query:" << query.str() << endl;
		if (!query)return false;
		if (!query.exec()) {
			cerr << "query执行失败，更新失败" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "更新失败，数据库被改变" << nNum << "条" << endl;
			return false;
		}
		cout << "更新成功，数据库被改变" << nNum << "条" << endl;
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
}
bool CDB::Search(UseQueryResult& res, mysqlpp::Query& query) {
	/*查询d_user表内的指定信息*/
	try {
		/*根据输入的数据库项和对应的值查询是否存在*/
		if (!query) {
			cout << "Query失效，请重新连接" << endl;
			return false;
		}
		if (!query)return false;
		res = query.use();
		if (!res) {
			cout << "Res失效，请重试" << endl;
			return false;
		}
	}
	catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return false;
	}
	catch (const BadConversion& er) {
		// Handle bad conversions
		cerr << "Conversion error: " << er.what() << endl <<
			"\tretrieved data size: " << er.retrieved <<
			", actual size: " << er.actual_size << endl;
		return false;
	}
	catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return false;
	}
	return true;
}