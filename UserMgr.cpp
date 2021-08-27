#include "User.h"
#include "UserMgr.h"
#include "CardMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	Log("������CUserMgr�Ĺ��캯��\n");
	m_mapByAccount.clear();
	m_mapById.clear();
}
CUserMgr::~CUserMgr() {
	Log("������CUserMgr����������\n");
}

bool CUserMgr::Add(const string& stdAccount) {
	long long int i64Id = g_UserMgr.GetOnlineId(stdAccount);
	if (i64Id != 0) {
		Log("���û��ѵ��룡\n");
		return false;
	}

	mysqlpp::Row row;
	if (!g_UserMgr.CheckExistInDB(row, stdAccount)) {/*����Ƿ������ݿ��д��ڣ����������½�һ��*/
		Log( "�û����������ݿⷢ������\n" );
		return false;
	}

	unique_ptr<CUser> pUser(new CUser(""));
	if (!pUser) {
		Log( "�����û�ʵ��ʧ��\n" );
		return false;
	}

	if (!row) {
		Log("����û�л�ȡ�����ݿ��е����ݷ��ص�Row\n");
		return false;
	}

	if (!pUser->Init(row)) {
		Log("�û���ʼ��ʧ��\n");
		return false;
	}

	string strAccount= pUser->GetAccount();
	i64Id= pUser->GetId();
	m_mapByAccount[strAccount] = i64Id;
	m_mapById[i64Id] = pUser.release();

	if (m_mapByAccount[strAccount] == 0 || m_mapById[i64Id] == 0) {
		Log("�û�����δ��Ч\n");
		Del(strAccount);
		return false;
	}
	return true;
}
bool CUserMgr::Del(const string& _strAccount) {
	/*����Accountɾ����Ӧ��User*/
	UserAccountMapIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end()) {
		Log("�û������� Account:"+ _strAccount+"\n");
		return false;
	}
	long long int i64Id = iterByAccount->second;
	m_mapByAccount.erase(iterByAccount);

	UserIdMapIter iterById = m_mapById.find(i64Id);
	if (iterById == m_mapById.end()) {
		Log( "�û������� ID:" +to_string( iterByAccount->second )+"\n");
		return false;
	}
	delete iterById->second;
	m_mapById.erase(iterById);
	return true;
}
bool CUserMgr::CheckExistInDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*����û��Ƿ���������ݿ�*/
	if (!SearchDB(row, _strAccount)) {
		cout << "���ݿ����޴��˻�" << endl;
		cout << "�½��û��У�\n" << endl;
		if (!Generate(_strAccount))
			return false;
		if (!SearchDB(row, _strAccount)) {
			cout << "����ע��ɹ������ݿ������޴��˻�" << endl;
			return false;
		}
	}
	return true;
}
CUser* CUserMgr::Get(const long long int _i64Id) {
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	UserIdMapIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		Log("Do not Find ID:" + to_string( _i64Id )+"\n");
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
long long int CUserMgr::GetOnlineId(const std::string& _strAccount) {/*�������һ��0˵�����û�������*/
	UserAccountMapIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())
		return 0;
	return iterByAccount->second;
}

void CUserMgr::PrintOnline() {
	/*��ӡ��ʾ����������ҵ���Ϣ*/
	for(auto &iter:m_mapById) {
		CUser* pUser = iter.second;
		if (!pUser) {
			continue;
		}
		cout << "ID:" << pUser->GetId() << "\tAccount:" << pUser->GetAccount() << "\tName:" << pUser->GetName() << "\tLev:" << pUser->GetLev()<<endl;
		CCardMgr &CardMgr = pUser->GetCardMgr();
		CardMgr.PrintAll();
	}
}
/*
* �����Ƕ�˽�������Ĳ��������ܱ�¶�����
*/


void CUserMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	m_mapByAccount.clear();
	for (auto &iter:m_mapById) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapById.clear();
}
bool CUserMgr::SearchDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CUserMgr::SearchUser()\n");
		if (!*pQuery) {
			Log("CUserMgr::SearchUser()  Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}
		*pQuery << "select * from d_user where account = %0q:value;";
		pQuery->parse();
		pQuery->template_defaults["value"] = _strAccount.c_str();
		bool bRet = g_DB.Search(res, *pQuery);

		Log("Query:" + pQuery->str() + "\n");
		if (!bRet) {
			Log("CUserMgr::SearchUser()  �����ݿ��ѯ�û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CUserMgr::SearchUser()  �����ݿ��ѯ�û��ɹ�\n");
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CUserMgr::SearchUser()  Query error: " + string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CUserMgr::SearchUser()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CUserMgr::SearchUser()  Error: " + string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CUserMgr::SearchUser()  Error: " +string( er.what() )+"\n");
		return false;
	}
	return true;
}

bool CUserMgr::Generate(const string& _strAccount) {
	/*����Ϊaccount��name,����CUser��InsertUser*/
	CUser user(_strAccount);
	string strName;
	cout << "�������ǳ�" << endl;
	getline(cin, strName);
	user.SetName(strName);
	if (!user.Insert()) {
		cout << "�û�ע��ʧ��" << endl;
		return false;
	}
	cout << "�û�ע��ɹ�" << endl;
	return true;
}