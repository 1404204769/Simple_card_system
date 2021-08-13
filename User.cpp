#include "User.h"
#include "CardMgr.h"
using namespace std;
CUser::CUser(const std::string& _strAccount) {
	m_strAccount = _strAccount;
	Log("������CUser���캯��\n");
}

CUser::~CUser()
{
	Log( "������CUser����������\n");

	if (!Update(*this))
		Log("�û����ݱ���ʧ��\n");
	else
		Log("�û����ݱ���ɹ�\n");
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
* ���������ݿ����ؽӿ�
*/
bool CUser::Insert() {
	/*�����ݿ��в����µ�User����*/
	try
	{
		Log( "CUser::InsertUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::Insert() Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "insert into d_user(account,name) values(%0q:account, %1q:name)";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();
		pQuery->template_defaults["name"] = m_strName.c_str();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Insert(*pQuery)) {
			Log("CUser::Insert() �����ݿ�������û�ʧ��\n");
			return false;
		}
		Log("CUser::Insert() �����ݿ�������û��ɹ�\n");
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
	/*�����ݿ���ɾ��ָ��User����*/
	try
	{
		Log("CUser::DeleteUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::Delete() Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "delete from d_user where account = %0q:account;";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CUser::Delete() �����ݿ�ɾ���û�ʧ��\n");
			return false;
		}
		Log("CUser::Delete() �����ݿ�ɾ���û��ɹ�\n");

		if (!m_pCardMgr->DelAll()) {
			Log("CUser::Delete() �����ݿ�ɾ���û����еĿ���ʧ��\n");
			return false;
		}
		Log("CUser::Delete() �����ݿ�ɾ���û����еĿ��Ƴɹ�\n");
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
	/*�����ݿ��и���ָ��User*/
	try
	{
		Log("CUser::UpdateUser()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!pQuery) {
			Log("CUser::UpdateUser()  Query���󲻴��ڣ��޷���������\n");
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
			Log("CUser::UpdateUser()�����ݿ�����û�ʧ��\n");
			return false;
		}
		Log("CUser::UpdateUser()�����ݿ�����û��ɹ�\n");
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
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/

		m_strAccount = row["account"];
		m_strCreateTime = row["create_time"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_unLev = row["lev"];
		m_i64Id = row["id"];
		m_pCardMgr = new CCardMgr();

		if (!m_pCardMgr->Init(this)) {
			cout << "�û�ӵ�еĿ��Ƽ���ʧ��" << endl;
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