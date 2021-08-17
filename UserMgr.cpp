#include "User.h"
#include "UserMgr.h"
#include "CardMgr.h"
using namespace std;
using namespace mysqlpp;
CUserMgr::CUserMgr() {
	Log("调用了CUserMgr的构造函数\n");

	m_mapById.clear();
	m_mapByAccount.clear();
}
CUserMgr::~CUserMgr() {
	Log("调用了CUserMgr的析构函数\n");

	Free();
}

bool CUserMgr::Add(const string& stdAccount) {
	long long int i64Id = g_UserMgr.GetOnlineId(stdAccount);
	if (i64Id != 0) {
		cout << "该用户已登入！" << endl;
		return false;
	}

	mysqlpp::Row row;
	if (!g_UserMgr.CheckExistInDB(row, stdAccount)) {/*检查是否在数据库中存在，不存在则新建一个*/
		cout << "用户数据在数据库发生错误" << endl;
		return false;
	}

	unique_ptr<CUser> pUser(new CUser(""));
	if (!pUser) {
		cout << "创建用户实体失败" << endl;
		return false;
	}

	if (!row) {
		cout << "错误：没有获取到数据库中的数据返回的Row" << endl;
		return false;
	}

	if (!pUser->Init(row)) {
		cout << "用户初始化失败" << endl;
		return false;
	}

	string strAccount= pUser->GetAccount();
	i64Id= pUser->GetId();
	m_mapByAccount[strAccount] = i64Id;
	m_mapById[i64Id] = pUser.release();

	if (m_mapByAccount.count(strAccount) == 0 || m_mapById.count(i64Id) == 0) {
		cout << "用户登入未生效" << endl;
		Del(strAccount);
		return false;
	}
	return true;
}
bool CUserMgr::Del(const string& _strAccount) {
	/*根据Account删除对应的User*/
	UserAccountMapIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end()) {
		cout << "用户不在线 Account:" << _strAccount << endl;
		return false;
	}
	long long int i64Id = iterByAccount->second;
	m_mapByAccount.erase(iterByAccount);

	UserIdMapIter iterById = m_mapById.find(i64Id);
	if (iterById == m_mapById.end()) {
		cout << "用户不在线 ID:" << iterByAccount->second << endl;
		return false;
	}
	delete iterById->second;
	m_mapById.erase(iterById);
	return true;
}
bool CUserMgr::CheckExistInDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*检查用户是否存在于数据库*/
	if (!SearchDB(row, _strAccount)) {
		cout << "数据库中无此账户" << endl;
		cout << "新建用户中：\n" << endl;
		if (!Generate(_strAccount))
			return false;
		if (!SearchDB(row, _strAccount)) {
			cout << "错误：注册成功后数据库依旧无此账户" << endl;
			return false;
		}
	}
	return true;
}
CUser* CUserMgr::Get(const long long int _i64Id) {
	/*从map里面查询，返回查询对象指针*/
	UserIdMapIter iterById = m_mapById.find(_i64Id);
	if (iterById == m_mapById.end()) {
		cout << "Do not Find ID:" << _i64Id << endl;
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
long long int CUserMgr::GetOnlineId(const std::string& _strAccount) {/*如果返回一个0说明该用户不在线*/
	UserAccountMapIter iterByAccount = m_mapByAccount.find(_strAccount);
	if (iterByAccount == m_mapByAccount.end())
		return 0;
	return iterByAccount->second;
}

void CUserMgr::PrintOnline() {
	/*打印显示所有在线玩家的信息*/
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
* 以下是对私有容器的操作，不能暴露给外层
*/


void CUserMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for (auto &iter:m_mapById) {
		delete iter.second;//先释放内存
	}
	m_mapByAccount.clear();
	m_mapById.clear();
}
bool CUserMgr::SearchDB(mysqlpp::Row& row, const std::string& _strAccount) {
	/*调用CDB函数查找指定对象,返回查询结果，如果引用的CUser对象的ID为0则说明数据库中无此数据*/
	try
	{
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CUserMgr::SearchUser()\n");
		if (!*pQuery) {
			Log("CUserMgr::SearchUser()  Query实例指针错误\n");//打印在控制台
			return false;
		}
		*pQuery << "select * from d_user where account = %0q:value;";
		pQuery->parse();
		pQuery->template_defaults["value"] = _strAccount.c_str();
		bool bRet = g_DB.Search(res, *pQuery);

		Log("Query:" + pQuery->str() + "\n");
		if (!bRet) {
			Log("CUserMgr::SearchUser()  从数据库查询用户失败\n");//打印在控制台
			return false;
		}
		Log("CUserMgr::SearchUser()  从数据库查询用户成功\n");
		row = res.fetch_row();
		if (!row) {
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/
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
	/*参数为account，name,调用CUser的InsertUser*/
	CUser user(_strAccount);
	string strName;
	cout << "请输入昵称" << endl;
	getline(cin, strName);
	user.SetName(strName);
	if (!user.Insert()) {
		cout << "用户注册失败" << endl;
		return false;
	}
	cout << "用户注册成功" << endl;
	return true;
}