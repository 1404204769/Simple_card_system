#include "DB.h"
using namespace std;
using namespace mysqlpp;

CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	Log("������CDB�Ĺ��캯��\n");
}
CDB::~CDB()
{
	Log("������CDB����������\n");
}
bool CDB::InitConnect(const string& strDB, const string& strServer, const string& strUser, const string& strPassword, const int nPort) {
	try {
		/*�������ݿ�����*/
		m_strDB = strDB;
		m_strServer = strServer;
		m_strUser = strUser;
		m_strPassword = strPassword;
		m_nPort = nPort;

		if (!m_Conn.set_option(new mysqlpp::SetCharsetNameOption("latin1")))return false;//�����ַ�����Ϊuft8
		if (!m_Conn.set_option(new mysqlpp::ReconnectOption(true)))return false;
		if (!m_Conn.connect(m_strDB.c_str(), m_strServer.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_nPort)) {
			Log("CDB::InitConnect()  ���ݿ�����ʧ��\n");
			return false;
		}

		m_pQuery = new Query(m_Conn.query("set names latin1"));
		if (!m_pQuery)return false;
		if (!m_pQuery->exec()) {
			Log("CDB::InitConnect()  ����latin1��ʽʧ��\n");
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
			Log("����CDB::getQuery()��ȡQueryʧ��\n");
			return nullptr;
		}

		Log("����CDB::getQuery()��ȡQuery�ɹ�\n");
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		Log("CDB::Insert()\n");
		if (!query) {
			Log("CDB::Insert()  QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Insert()  queryִ��ʧ�ܣ�����ʧ��\n");
			return false;
		}

		long long int i64Num = query.affected_rows();
		if (i64Num == 0) {
			Log("CDB::Insert()  ����ʧ�ܣ����ݿⱻ�ı�" + to_string(i64Num) + "��\n");
			return false;
		}
		Log("CDB::Insert()  ����ɹ������ݿⱻ�ı�" + to_string(i64Num)+"��\n");
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		Log("CDB::Delete()\n");
		if (!query) {
			Log("CDB::Delete()  QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Delete()  queryִ��ʧ�ܣ�ɾ��ʧ��\n");
			return false;
		}

		long long int i64Num = query.affected_rows();
		if (i64Num == 0) {
			Log("CDB::Delete()  ɾ��ʧ�ܣ����ݿⱻ�ı�" +to_string(i64Num) + "��\n");
			return false;
		}
		Log("CDB::Delete()  ɾ���ɹ������ݿⱻ�ı�" +to_string(i64Num) + "��\n");
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
	/*�����ݿ��ж�Ӧ�������޸�*/
	try {
		if (!query) {
			Log("CDB::Update()  QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			Log("CDB::Update()  queryִ��ʧ�ܣ�����ʧ��\n");
			return false;
		}

		Log("CDB::Update()  ���³ɹ������ݿⱻ�ı�" + to_string(query.affected_rows())+"��\n");
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
	/*��ѯd_user���ڵ�ָ����Ϣ*/
	try {
		/*������������ݿ���Ͷ�Ӧ��ֵ��ѯ�Ƿ����*/
		if (!query) {
			Log("CDB::Search()  QueryʧЧ������������\n");
			return false;
		}
		res = query.use();
		if (!res) {
			Log("CDB::Search()  ResʧЧ��������\n");
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