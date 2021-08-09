#include "UserMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	cout << "������CUserMgr�Ĺ��캯��" << endl;
	m_mapById.clear();
	m_mapByAccount.clear();
}
CUserMgr::~CUserMgr() {
	cout << "������CUserMgr����������" << endl;
	Free();
}
bool CUserMgr::AddUser(const string& _strAccount ) {
	/*��׼���������û������ݱ�����Map��*/
	long long int i64Id = getId(_strAccount);
	if (i64Id != 0) {
		cout << "����ʧ��,���û��ѵ��룡" << endl;
		return false;
	}
	mysqlpp::Row row;
	if (!SearchUser(row, _strAccount)) {
		cout << "���ݿ����޴��˻�" << endl;
		cout << "�½��û��У�\n�������˻��ǳ�" << endl;
		string strName;
		getline(cin, strName);
		if(!GenerateUser(_strAccount,strName))
			return false;
		if (!SearchUser(row, _strAccount)) {
			cout << "����ע��ɹ������ݿ������޴��˻�" << endl;
			return false;
		}
	}
	CUser *pUser=new CUser(_strAccount);
	if (!pUser) {
		cout << "�����û�ʵ��ʧ��" << endl;
		return false;
	}
	if (!row) {
		cout << "����û�л�ȡ�����ݿ��е����ݷ��ص�Row" << endl;
		return false;
	}
	if (!pUser->InitUser(row)) {
		cout << "�û���ʼ��ʧ��" << endl;
		return false;
	}
	if (!AddToMap(pUser)) {
		cout << "�û��������߶���ʧ��" << endl;
		return false;
	}
	return true;  
}


bool CUserMgr::ReduceUser(const string& _strAccount) {
	return DeleteFromMap(_strAccount);
}
CUser* CUserMgr::getUser(const long long int _i64Id) {
	/*��map�����ѯ�����ز�ѯ����ָ��*/
	MapByIdIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find ID:" << _i64Id << endl;
		return nullptr;
	}
	return iterById->second;
}
long long int CUserMgr::getId(const std::string& _strAccount) {/*�������һ��0˵�����û�������*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())return 0;
	return iterByAccount->second;
}

/*
* �����Ƕ�˽�������Ĳ��������ܱ�¶�����
*/

bool CUserMgr::AddToMap(CUser* _pUser) {
	if (!_pUser)return false;
	m_mapByAccount[_pUser->getAccount()] = _pUser->getId();
	m_mapById[_pUser->getId()] = _pUser;
	if (m_mapByAccount.count(_pUser->getAccount()) == 0 || m_mapById.count(_pUser->getId()) == 0) {
		cout << "�û�����ʧ��" << endl;
		ReduceUser(_pUser->getAccount());
		return false;
	}
	return true;
}
bool CUserMgr::DeleteFromMap(const string& _strAccount) {
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
		mysqlpp::Query* pQuery = g_DB.getQuery();
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\n";
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "select * from d_user where account = %0q:value;";
		pQuery->parse();
		pQuery->template_defaults["value"] = _strAccount.c_str();
		bool bRet = g_DB.Search(res, *pQuery);
		//cout << "Query:" << pQuery->str() << endl;
		strIn << "Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			//cout << "�����ݿ��ѯ�û�ʧ��" << endl;
			strIn << "�����ݿ��ѯ�û�ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
			return false;
		}
		//cout << "�����ݿ��ѯ�û��ɹ�" << endl;
		strIn << "�����ݿ��ѯ�û��ɹ�\n";
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/
		OutputDebugPrintf(strIn.str().c_str());//��ӡ�ڿ���̨
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}

bool CUserMgr::GenerateUser(const string& _strAccount, const string& _strName) {
	/*����Ϊaccount��name,����CUser��InsertUser*/
	CUser user(_strAccount, _strName);
	if (!user.InsertUser()) {
		cout << "�û�ע��ʧ��" << endl;
		return false;
	}
	cout << "�û�ע��ɹ�" << endl;
	return true;
}