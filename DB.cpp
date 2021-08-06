#include "DB.h"
using namespace std;
using namespace mysqlpp;
CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	cout << "������CDB�Ĺ��캯��" << endl;
}
CDB::~CDB()
{
	cout << "������CDB����������" << endl;
	m_Conn.~Connection();
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
			//cout << m_Conn.error() << endl;
			cout << "���ݿ�����ʧ��" << endl;
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		if (!query) {
			cerr << "QueryʧЧ������������" << endl;
			return false;
		}
		if (!query.exec()) {
			cerr << "queryִ��ʧ�ܣ�����ʧ��" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "����ʧ�ܣ����ݿⱻ�ı�" << nNum << "��" << endl;
			return false;
		}
		cout << "����ɹ������ݿⱻ�ı�" << nNum << "��" << endl;
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		if (!query) {
			cerr << "QueryʧЧ������������" << endl;
			return false;
		}
		if (!query.exec()) {
			cerr << "queryִ��ʧ�ܣ�ɾ��ʧ��" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "ɾ��ʧ�ܣ����ݿⱻ�ı�"<< nNum<<"��" << endl;
			return false;
		}
		cout << "ɾ���ɹ������ݿⱻ�ı�" << nNum << "��" << endl;
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
	/*�����ݿ��ж�Ӧ�������޸�*/
	try {
		if (!query) {
			cout << "QueryʧЧ������������" << endl;
			return false;
		}
		//cout << "Query:" << query.str() << endl;
		if (!query)return false;
		if (!query.exec()) {
			cerr << "queryִ��ʧ�ܣ�����ʧ��" << endl;
			return false;
		}
		int nNum = query.affected_rows();
		if (query.affected_rows() == 0) {
			cout << "����ʧ�ܣ����ݿⱻ�ı�" << nNum << "��" << endl;
			return false;
		}
		cout << "���³ɹ������ݿⱻ�ı�" << nNum << "��" << endl;
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
	/*��ѯd_user���ڵ�ָ����Ϣ*/
	try {
		/*������������ݿ���Ͷ�Ӧ��ֵ��ѯ�Ƿ����*/
		if (!query) {
			cout << "QueryʧЧ������������" << endl;
			return false;
		}
		if (!query)return false;
		res = query.use();
		if (!res) {
			cout << "ResʧЧ��������" << endl;
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