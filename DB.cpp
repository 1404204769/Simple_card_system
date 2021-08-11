#include "DB.h"
using namespace std;
using namespace mysqlpp;

CDB::CDB() :m_Conn(true), m_strDB(""), m_strServer(""), m_strUser(""), m_strPassword(""), m_nPort(0)
{
	OutputDebugPrintf("������CDB�Ĺ��캯��");
}
CDB::~CDB()
{
	OutputDebugPrintf("������CDB����������");
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
			OutputDebugPrintf("CDB::InitConnect() -> ���ݿ�����ʧ��");
			return false;
		}
		Query query = m_Conn.query("set names latin1");
		if (!query)return false;
		if (!query.exec()) {
			OutputDebugPrintf("CDB::InitConnect() -> ����latin1��ʽʧ��");
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
			OutputDebugPrintf("����CDB::getQuery()��ȡQueryʧ��");
			return nullptr;
		}
		OutputDebugPrintf("����CDB::getQuery()��ȡQuery�ɹ�");
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Insert()->QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Insert()->queryִ��ʧ�ܣ�����ʧ��\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn <<"CDB::Insert()->";
		if (query.affected_rows() == 0) {
			strIn<< "����ʧ�ܣ����ݿⱻ�ı�" << i64Num<<"��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn<<"����ɹ������ݿⱻ�ı�" <<i64Num<<"��\n";
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
	/*����һ���µ�ֵ�����ݿ���*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Delete()->QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Delete()->queryִ��ʧ�ܣ�ɾ��ʧ��\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn<< "CDB::Delete()->";
		if (query.affected_rows() == 0) {
			strIn<<"ɾ��ʧ�ܣ����ݿⱻ�ı�" << i64Num<< "��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn<< "ɾ���ɹ������ݿⱻ�ı�" << i64Num<< "��\n";
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
	/*�����ݿ��ж�Ӧ�������޸�*/
	try {
		if (!query) {
			OutputDebugPrintf("CDB::Update()->QueryʧЧ������������\n");
			return false;
		}
		if (!query.exec()) {
			OutputDebugPrintf("CDB::Update()->queryִ��ʧ�ܣ�����ʧ��\n");
			return false;
		}
		long long int i64Num = query.affected_rows();
		stringstream strIn;
		strIn<<"CDB::Delete()-> ���³ɹ������ݿⱻ�ı�" + query.affected_rows()<<"��\n";
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
	/*��ѯd_user���ڵ�ָ����Ϣ*/
	try {
		/*������������ݿ���Ͷ�Ӧ��ֵ��ѯ�Ƿ����*/
		if (!query) {
			OutputDebugPrintf("CDB::Search()->QueryʧЧ������������\n");
			return false;
		}
		res = query.use();
		if (!res) {
			OutputDebugPrintf("CDB::Search()->ResʧЧ��������\n");
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