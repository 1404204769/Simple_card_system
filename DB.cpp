#include "DB.h"
using namespace std;
using namespace mysqlpp;

CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	OutputDebugPrintf("调用了CDB的构造函数");
}
CDB::~CDB()
{
	OutputDebugPrintf("调用了CDB的析构函数");
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
			OutputDebugPrintf("CDB::InitConnect() -> 数据库链接失败");
			return false;
		}
		Query query = m_Conn.query("set names latin1");
		if (!query)return false;
		if (!query.exec()) {
			OutputDebugPrintf("CDB::InitConnect() -> 设置latin1格式失败");
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn <<  "CDB::InitConnect()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CDB::InitConnect()\nConversion error: "<< er.what()<< "\ntretrieved data size: "<< er.retrieved << ", actual size: " << er.actual_size<<"\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn<<"CDB::InitConnect()\nError: "<< er.what()<< "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
mysqlpp::Query* CDB::GetQuery() {
	try {
		Query* pQuery = new Query(m_Conn.query());
		if (!pQuery) {
			OutputDebugPrintf("调用CDB::getQuery()获取Query失败");
			return nullptr;
		}
		OutputDebugPrintf("调用CDB::getQuery()获取Query成功");
		return pQuery;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		stringstream strIn;
		strIn<< "CDB::getQuery()\nQuery error: "<< er.what()<< "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		stringstream strIn;
		strIn <<  "CDB::getQuery()\nConversion error: "<< er.what()<< "\ntretrieved data size: "<< er.retrieved << ", actual size: " << er.actual_size<< "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn <<  "CDB::getQuery()\nError: "<< er.what()<< "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	return nullptr;
}
bool CDB::Insert(mysqlpp::Query&query) {
	/*插入一条新的值到数据库中*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Insert()->Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Insert()->query执行失败，插入失败\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn <<"CDB::Insert()->";
		if (query.affected_rows() == 0) {
			strIn<< "插入失败，数据库被改变" << i64Num<<"条\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn<<"插入成功，数据库被改变" <<i64Num<<"条\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CDB::Insert()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CDB::Insert()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CDB::Insert()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
bool CDB::Delete(mysqlpp::Query& query) {
	/*插入一条新的值到数据库中*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Delete()->Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Delete()->query执行失败，删除失败\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn<< "CDB::Delete()->";
		if (query.affected_rows() == 0) {
			strIn<<"删除失败，数据库被改变" << i64Num<< "条\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn<< "删除成功，数据库被改变" << i64Num<< "条\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CDB::Delete()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CDB::Delete()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CDB::Delete()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
bool CDB::Update(mysqlpp::Query& query) {
	/*将数据库中对应的数据修改*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Update()->Query失效，请重新连接\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Update()->query执行失败，更新失败\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn<<"CDB::Delete()-> 更新成功，数据库被改变" + query.affected_rows()<<"条\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CDB::Update()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CDB::Update()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CDB::Update()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
bool CDB::Search(UseQueryResult& res, mysqlpp::Query& query) {
	/*查询d_user表内的指定信息*/
	try {
		/*根据输入的数据库项和对应的值查询是否存在*/
		if (!query) {
			OutputDebugPrintf("CDB::Search()->Query失效，请重新连接\n");
			return false;
		}
		res = query.use();
		if (!res) {
			OutputDebugPrintf("CDB::Search()->Res失效，请重试\n");
			return false;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CDB::Search()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CDB::Search()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CDB::Search()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}