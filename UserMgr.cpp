#include "UserMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	stringstream strIn;
	strIn << "调用了CUserMgr的构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	m_mapById.clear();
	m_mapByAccount.clear();
}
CUserMgr::~CUserMgr() {
	stringstream strIn;
	strIn << "调用了CUserMgr的析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	Free();
}

bool CUserMgr::DelUser(const string& _strAccount) {
	/*根据Account删除对应的User*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end()) {
		cout << "用户不在线 Account:" << _strAccount << endl;
		return false;
	}
	long long int i64Id = iterByAccount->second;
	m_mapByAccount.erase(iterByAccount);
	MapByIdIter iterById = m_mapById.find(i64Id);
	if (iterById == m_mapById.end()) {
		cout << "用户不在线 ID:" << iterByAccount->second << endl;
		return false;
	}
	delete iterById->second;
	iterById->second = nullptr;
	m_mapById.erase(iterById);
	return true;
}
bool CUserMgr::CheckExistInDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*检查用户是否存在于数据库*/
	if (!SearchUser(row, _strAccount)) {
		cout << "数据库中无此账户" << endl;
		cout << "新建用户中：\n" << endl;
		if (!GenerateUser(_strAccount))
			return false;
		if (!SearchUser(row, _strAccount)) {
			cout << "错误：注册成功后数据库依旧无此账户" << endl;
			return false;
		}
	}
	return true;
}
CUser* CUserMgr::GetUser(const long long int _i64Id) {
	/*从map里面查询，返回查询对象指针*/
	MapByIdIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find ID:" << _i64Id << endl;
		return nullptr;
	}
	return iterById->second;
}
long long int CUserMgr::GetOnlineUserId(const std::string& _strAccount) {/*如果返回一个0说明该用户不在线*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())return 0;
	return iterByAccount->second;
}

void CUserMgr::PrintOnlineUser() {
	/*打印显示所有在线玩家的信息*/
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
* 以下是对私有容器的操作，不能暴露给外层
*/

bool CUserMgr::AddUser(mysqlpp::Row& row) {
	CUser* pUser = new CUser("");
	if (!pUser) {
		cout << "创建用户实体失败" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	if (!row) {
		cout << "错误：没有获取到数据库中的数据返回的Row" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	if (!pUser->InitUser(row)) {
		cout << "用户初始化失败" << endl;
		delete pUser;
		pUser = nullptr;
		return false;
	}
	m_mapByAccount[pUser->GetAccount()] = pUser->GetId();
	m_mapById[pUser->GetId()] = pUser;
	if (m_mapByAccount.count(pUser->GetAccount()) == 0 || m_mapById.count(pUser->GetId()) == 0) {
		cout << "用户登入未生效" << endl;
		DelUser(pUser->GetAccount());
		return false;
	}
	pUser = nullptr;
	return true;
}

void CUserMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	MapByAccountIter iterByAccount = m_mapByAccount.begin();
	MapByIdIter iterById = m_mapById.begin();
	while (iterById != m_mapById.end()) {
		delete iterById->second;//先释放内存
		iterById->second = nullptr;//置空
		iterById++;
	}
	m_mapByAccount.clear();
	m_mapById.clear();
}
bool CUserMgr::SearchUser(mysqlpp::Row& row, const std::string& _strAccount) {
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\n";
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//打印在控制台
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
			strIn << "从数据库查询用户失败\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//打印在控制台
			return false;
		}
		strIn << "从数据库查询用户成功\n";
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());//打印在控制台
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
	/*参数为account，name,调用CUser的InsertUser*/
	CUser user(_strAccount);
	string strName;
	cout << "请输入昵称" << endl;
	getline(cin, strName);
	user.SetName(strName);
	if (!user.InsertUser()) {
		cout << "用户注册失败" << endl;
		return false;
	}
	cout << "用户注册成功" << endl;
	return true;
}