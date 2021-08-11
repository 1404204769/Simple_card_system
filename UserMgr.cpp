#include "UserMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	stringstream strIn;
	strIn << "������CUserMgr�Ĺ��캯��\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	m_mapById.clear();
	m_mapByAccount.clear();
}
CUserMgr::~CUserMgr() {
	stringstream strIn;
	strIn << "������CUserMgr����������\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	Free();
}

bool CUserMgr::DelUser(const string& _strAccount) {
	/*����Accountɾ����Ӧ��User*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end()) {
		cout << "�û������� Account:" << _strAccount << endl;
		return false;
	}
	long long int i64Id = iterByAccount->second;
	m_mapByAccount.erase(iterByAccount);
	MapByIdIter iterById = m_mapById.find(i64Id);
	if (iterById == m_mapById.end()) {
		cout << "�û������� ID:" << iterByAccount->second << endl;
		return false;
	}
	delete iterById->second;
	iterById->second = nullptr;
	m_mapById.erase(iterById);
	return true;
}
bool CUserMgr::CheckExistInDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*����û��Ƿ���������ݿ�*/
	if (!SearchUser(row, _strAccount)) {
		cout << "���ݿ����޴��˻�" << endl;
		cout << "�½��û��У�\n" << endl;
		if (!GenerateUser(_strAccount))
			return false;
		if (!SearchUser(row, _strAccount)) {
			cout << "����ע��ɹ������ݿ������޴��˻�" << endl;
			return false;
		}
	}
	return true;
}
CUser* CUserMgr::GetUser(const long long int _i64Id) {
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	MapByIdIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find ID:" << _i64Id << endl;
		return nullptr;
	}
	return iterById->second;
}
long long int CUserMgr::GetOnlineUserId(const std::string& _strAccount) {/*�������һ��0˵�����û�������*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())return 0;
	return iterByAccount->second;
}

void CUserMgr::PrintOnlineUser() {
	/*��ӡ��ʾ����������ҵ���Ϣ*/
	MapByIdIter iterById = m_mapById.begin();
	while (iterById != m_mapById.end()) {
		CUser* pUser = iterById->second;
		iterById++;
		if (!pUser) {
			pUser = nullptr;
			continue;
		}
		cout << "ID:" << pUser->GetId() << "\tAccount:" << pUser->GetAccount() << "\tName:" << pUser->GetName() << "\tLev:" << pUser->GetLev()<<endl;
		CCardMgr* pCardMgr = pUser->GetCardMgr();
		if (!pCardMgr) {
			pUser = nullptr;
			pCardMgr = nullptr;
			continue;
		}
		pCardMgr->PrintAllCard();
		pUser = nullptr;
		pCardMgr = nullptr;
	}
}
/*
* �����Ƕ�˽�������Ĳ��������ܱ�¶�����
*/

bool CUserMgr::AddUser(mysqlpp::Row& row) {
	CUser* pUser = new CUser("");
	if (!pUser) {
		cout << "�����û�ʵ��ʧ��" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	if (!row) {
		cout << "����û�л�ȡ�����ݿ��е����ݷ��ص�Row" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	if (!pUser->InitUser(row)) {
		cout << "�û���ʼ��ʧ��" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	m_mapByAccount[pUser->GetAccount()] = pUser->GetId();
	m_mapById[pUser->GetId()] = pUser;
	if (m_mapByAccount.count(pUser->GetAccount()) == 0 || m_mapById.count(pUser->GetId()) == 0) {
		cout << "�û�����δ��Ч" << endl;
		DelUser(pUser->GetAccount());
		return false;
	}
	pUser = nullptr;
	return true;
}

void CUserMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	MapByAccountIter iterByAccount = m_mapByAccount.begin();
	MapByIdIter iterById = m_mapById.begin();
	while (iterById != m_mapById.end()) {
		delete iterById->second;//���ͷ��ڴ�
		iterById->second = nullptr;//�ÿ�
		iterById++;
	}
	m_mapByAccount.clear();
	m_mapById.clear();
}
bool CUserMgr::SearchUser(mysqlpp::Row& row, const std::string& _strAccount) {
	/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\n";
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "select * from d_user where account = %0q:value;";
		pQuery->parse();
		pQuery->template_defaults["value"] = _strAccount.c_str();
		bool bRet = g_DB.Search(res, *pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			strIn << "�����ݿ��ѯ�û�ʧ��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
			return false;
		}
		strIn << "�����ݿ��ѯ�û��ɹ�\n";
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}

bool CUserMgr::GenerateUser(const string& _strAccount) {
	/*����Ϊaccount��name,����CUser��InsertUser*/
	CUser user(_strAccount);
	string strName;
	cout << "�������ǳ�" << endl;
	getline(cin, strName);
	user.SetName(strName);
	if (!user.InsertUser()) {
		cout << "�û�ע��ʧ��" << endl;
		return false;
	}
	cout << "�û�ע��ɹ�" << endl;
	return true;
}