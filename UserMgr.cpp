#include "UserMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	cout << "调用了CUserMgr的构造函数" << endl;
	m_mapById.clear();
	m_mapByAccount.clear();
}
CUserMgr::~CUserMgr() {
	cout << "调用了CUserMgr的析构函数" << endl;
	Free();
}
bool CUserMgr::AddUser(const string& _strAccount ) {
	/*将准备新增的用户的数据保存在Map中*/
	long long int i64Id = getId(_strAccount);
	if (i64Id != 0) {
		cout << "登入失败,该用户已登入！" << endl;
		return false;
	}
	mysqlpp::Row row;
	if (!SearchUser(row, _strAccount)) {
		cout << "数据库中无此账户" << endl;
		cout << "新建用户中：\n请输入账户昵称" << endl;
		string strName;
		getline(cin, strName);
		if(!GenerateUser(_strAccount,strName))
			return false;
		if (!SearchUser(row, _strAccount)) {
			cout << "错误：注册成功后数据库依旧无此账户" << endl;
			return false;
		}
	}
	CUser *pUser=new CUser(_strAccount);
	if (!pUser) {
		cout << "创建用户实体失败" << endl;
		return false;
	}
	if (!row) {
		cout << "错误：没有获取到数据库中的数据返回的Row" << endl;
		return false;
	}
	if (!pUser->InitUser(row)) {
		cout << "用户初始化失败" << endl;
		return false;
	}
	if (!AddToMap(pUser)) {
		cout << "用户加入在线队列失败" << endl;
		return false;
	}
	return true;  
}


bool CUserMgr::ReduceUser(const string& _strAccount) {
	return DeleteFromMap(_strAccount);
}
CUser* CUserMgr::getUser(const long long int _i64Id) {
	/*从map里面查询，返回查询对象指针*/
	MapByIdIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find ID:" << _i64Id << endl;
		return nullptr;
	}
	return iterById->second;
}
long long int CUserMgr::getId(const std::string& _strAccount) {/*如果返回一个0说明该用户不在线*/
	MapByAccountIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())return 0;
	return iterByAccount->second;
}

/*
* 以下是对私有容器的操作，不能暴露给外层
*/

bool CUserMgr::AddToMap(CUser* _pUser) {
	if (!_pUser)return false;
	m_mapByAccount[_pUser->getAccount()] = _pUser->getId();
	m_mapById[_pUser->getId()] = _pUser;
	if (m_mapByAccount.count(_pUser->getAccount()) == 0 || m_mapById.count(_pUser->getId()) == 0) {
		cout << "用户登入失败" << endl;
		ReduceUser(_pUser->getAccount());
		return false;
	}
	return true;
}
bool CUserMgr::DeleteFromMap(const string& _strAccount) {
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
		mysqlpp::Query* pQuery = g_DB.getQuery();
		stringstream strIn;
		strIn << "CUserMgr::SearchUser()\n";
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
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
			//cout << "从数据库查询用户失败" << endl;
			strIn << "从数据库查询用户失败\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
			return false;
		}
		//cout << "从数据库查询用户成功" << endl;
		strIn << "从数据库查询用户成功\n";
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/
		OutputDebugPrintf(strIn.str().c_str());//打印在控制台
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
	/*参数为account，name,调用CUser的InsertUser*/
	CUser user(_strAccount, _strName);
	if (!user.InsertUser()) {
		cout << "用户注册失败" << endl;
		return false;
	}
	cout << "用户注册成功" << endl;
	return true;
}