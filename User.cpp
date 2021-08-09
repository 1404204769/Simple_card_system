#include "User.h"
using namespace std;
CUser::CUser(const std::string& _strAccount) {
	m_strAccount = _strAccount;
	stringstream strIn;
	strIn << "调用了CUser构造函数\n";
	OutputDebugPrintf(strIn.str().c_str());
}

CUser::~CUser()
{
	stringstream strIn;
	strIn<< "调用了CUser的析构函数\n";
	//cout << "调用了CUser的析构函数" << endl;
	bool bRet=UpdateUser(*this);
	if(!bRet)
		strIn << "用户数据保存失败\n";
	else 
		strIn << "用户数据保存成功\n";
	OutputDebugPrintf(strIn.str().c_str());
	//cout << "用户数据保存："<< UpdateUser(*this)<< endl;
}

//CUser::CUser(mysqlpp::Row& row) {
//	try {
//		if (!row) {
//			OutputDebugPrintf("没有找到Row对象，构造默认CUser(ID为0)");
//			return;
//		}
//		m_strAccount = row["account"];
//		m_strCreateTime = row["create_time"];
//		m_strName = row["name"];
//		m_i64Exp = row["exp"];
//		m_unLev = row["lev"];
//		m_i64Id = row["id"];
//		stringstream strIn;
//		strIn << "CUser::CUser()\n 调用了CUser的构造函数\n 增加了(ID:" + m_i64Id << ",Account:" << m_strAccount << ",CreateTime:" << m_strCreateTime << ",Name:" << m_strName << ",Exp:" << m_i64Exp << ",Lev:" << m_i64Exp <<"\n";
//		OutputDebugPrintf(strIn.str().c_str());
//	}
//	catch (const mysqlpp::BadQuery& er) {
//		stringstream strIn;
//		strIn<< "CUser::CUser()\nQuery error: "<<er.what()<<"\n";
//		OutputDebugPrintf(strIn.str().c_str());
//		return;
//	}
//	catch (const mysqlpp::BadConversion& er) {
//		stringstream strIn;
//		strIn<<  "CUser::CUser()\nConversion error: "<< er.what()<< "\ntretrieved data size: "<<er.retrieved<<", actual size: "<< er.actual_size<<"\n";
//		OutputDebugPrintf(strIn.str().c_str());
//		return;
//	}
//	catch (const mysqlpp::BadIndex& er) {
//		stringstream strIn;
//		strIn<< "CUser::CUser()\nError: "<<er.what()<<"\n";
//		OutputDebugPrintf(strIn.str().c_str());
//		return;
//	}
//	catch (const mysqlpp::Exception& er) {
//		stringstream strIn;
//		strIn<< "CUser::CUser()\nError: "<<er.what()<<"\n";
//		OutputDebugPrintf(strIn.str().c_str());
//		return;
//	}
//}

const std::string& CUser::getAccount() {
	return m_strAccount;
}
const std::string& CUser::getCreateTime() {
	return m_strCreateTime;
}
const std::string& CUser::getName() {
	return m_strName;
}
const long long int CUser::getId() {
	return m_i64Id;
}
const long long int CUser::getExp() {
	return m_i64Exp;
}
const unsigned int CUser::getLev() {
	return m_unLev;
}

void CUser::setLev(const unsigned int _unLev) {
	m_unLev = _unLev;
}
void CUser::setName(const string& _strName) {
	m_strName = _strName;
}
void CUser::setExp(const long long int _i64Exp) {
	m_i64Exp = _i64Exp;
}

/*
* 以下是数据库层相关接口
*/
bool CUser::InsertUser() {
	/*在数据库中插入新的User数据*/
	try
	{
		stringstream strIn;
		strIn<< "CUser::InsertUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			strIn<< "Query实例指针错误\n";
			OutputDebugPrintf(strIn.str().c_str());
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "insert into d_user(account,name) values(%0q:account, %1q:name)";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();
		pQuery->template_defaults["name"] = m_strName.c_str();
		bool bRet = g_DB.Insert(*pQuery);
		strIn<<"Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			//cout << "往数据库插入新用户失败" << endl;
			strIn<<"往数据库插入新用户失败\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		//cout << "往数据库插入新用户成功" << endl;
		strIn<< "往数据库插入新用户成功\n";
		OutputDebugPrintf(strIn.str().c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUser::InsertUser()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUser::InsertUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUser::InsertUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUser::InsertUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}
bool CUser::DeleteUser() {
	/*在数据库中删除指定User数据*/
	try
	{
		stringstream strIn;
		strIn<<"CUser::DeleteUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			strIn<< "Query实例指针错误\n";
			OutputDebugPrintf(strIn.str().c_str());
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "delete from d_user where account = %0q:account;";
		pQuery->parse();
		pQuery->template_defaults["account"] = m_strAccount.c_str();
		bool bRet = g_DB.Delete(*pQuery);
		strIn<< "Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			strIn<<"从数据库删除用户失败\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		strIn<< "从数据库删除用户成功\n";
		OutputDebugPrintf(strIn.str().c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUser::DeleteUser()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUser::DeleteUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUser::DeleteUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUser::DeleteUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}
bool CUser::UpdateUser(CUser& user) {
	/*在数据库中更新指定User*/
	try
	{
		stringstream strIn;
		strIn<<"CUser::UpdateUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			delete pQuery;
			pQuery = nullptr;
			strIn<<"User对象不存在，无法更新数据\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		*pQuery << "update `d_user` set account=%0q:account,`name`=%1q:name,exp=%2:exp,lev=%3:lev where id=%4:id;";
		pQuery->parse();
		pQuery->template_defaults["account"] = user.m_strAccount.c_str();
		pQuery->template_defaults["name"] = user.m_strName.c_str();
		pQuery->template_defaults["exp"] = user.m_i64Exp;
		pQuery->template_defaults["lev"] = user.m_unLev;
		pQuery->template_defaults["id"] = user.m_i64Id;
		strIn<< "Query:" << pQuery->str()<<"\n";
		bool bRet = g_DB.Update(*pQuery);
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			//cout << "向数据库更新用户失败" << endl;
			strIn<<"向数据库更新用户失败\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		//cout << "向数据库更新用户成功" << endl;
		strIn<< "向数据库更新用户成功\n";
		OutputDebugPrintf(strIn.str().c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUser::UpdateUser()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUser::UpdateUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUser::UpdateUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUser::UpdateUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;

}

bool CUser::InitUser(mysqlpp::Row& row) {
	/*根据查询获取的mysqlpp::Row对User对象初始化*/
	try
	{
		if (!row) {
			OutputDebugPrintf("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/
		m_strAccount = row["account"];
		m_strCreateTime = row["create_time"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_unLev = row["lev"];
		m_i64Id = row["id"];
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CUser::InitUser()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CUser::InitUser()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CUser::InitUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CUser::InitUser()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}