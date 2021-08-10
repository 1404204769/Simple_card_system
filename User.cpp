#include "User.h"
using namespace std;
CUser::CUser(const std::string& _strAccount) {
	m_strAccount = _strAccount;
	stringstream strIn;
	strIn << "������CUser���캯��\n";
	OutputDebugPrintf(strIn.str().c_str());
}

CUser::~CUser()
{
	stringstream strIn;
	strIn<< "������CUser����������\n";
	//cout << "������CUser����������" << endl;
	bool bRet=UpdateUser(*this);
	if(!bRet)
		strIn << "�û����ݱ���ʧ��\n";
	else 
		strIn << "�û����ݱ���ɹ�\n";
	delete m_pCardMgr;
	m_pCardMgr = nullptr;
	OutputDebugPrintf(strIn.str().c_str());

	//cout << "�û����ݱ��棺"<< UpdateUser(*this)<< endl;
}

//CUser::CUser(mysqlpp::Row& row) {
//	try {
//		if (!row) {
//			OutputDebugPrintf("û���ҵ�Row���󣬹���Ĭ��CUser(IDΪ0)");
//			return;
//		}
//		m_strAccount = row["account"];
//		m_strCreateTime = row["create_time"];
//		m_strName = row["name"];
//		m_i64Exp = row["exp"];
//		m_unLev = row["lev"];
//		m_i64Id = row["id"];
//		stringstream strIn;
//		strIn << "CUser::CUser()\n ������CUser�Ĺ��캯��\n ������(ID:" + m_i64Id << ",Account:" << m_strAccount << ",CreateTime:" << m_strCreateTime << ",Name:" << m_strName << ",Exp:" << m_i64Exp << ",Lev:" << m_i64Exp <<"\n";
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
CCardMgr* CUser::getCardMgr() {
	return m_pCardMgr;
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
* ���������ݿ����ؽӿ�
*/
bool CUser::InsertUser() {
	/*�����ݿ��в����µ�User����*/
	try
	{
		stringstream strIn;
		strIn<< "CUser::InsertUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			strIn<< "Queryʵ��ָ�����\n";
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
			//cout << "�����ݿ�������û�ʧ��" << endl;
			strIn<<"�����ݿ�������û�ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		//cout << "�����ݿ�������û��ɹ�" << endl;
		strIn<< "�����ݿ�������û��ɹ�\n";
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
	/*�����ݿ���ɾ��ָ��User����*/
	try
	{
		stringstream strIn;
		strIn<<"CUser::DeleteUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			strIn<< "Queryʵ��ָ�����\n";
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
			strIn<<"�����ݿ�ɾ���û�ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		strIn<< "�����ݿ�ɾ���û��ɹ�\n";
		if (!m_pCardMgr->DelAllCard()) {
			strIn << "�����ݿ�ɾ���û����еĿ���ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		strIn << "�����ݿ�ɾ���û����еĿ��Ƴɹ�\n";
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
	/*�����ݿ��и���ָ��User*/
	try
	{
		stringstream strIn;
		strIn<<"CUser::UpdateUser()\n";
		mysqlpp::Query* pQuery = g_DB.getQuery();
		if (!*pQuery) {
			delete pQuery;
			pQuery = nullptr;
			strIn<<"Query���󲻴��ڣ��޷���������\n";
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
			//cout << "�����ݿ�����û�ʧ��" << endl;
			strIn<<"�����ݿ�����û�ʧ��\n";
			OutputDebugPrintf(strIn.str().c_str());
			return false;
		}
		//cout << "�����ݿ�����û��ɹ�" << endl;
		strIn<< "�����ݿ�����û��ɹ�\n";
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
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			OutputDebugPrintf("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/
		m_strAccount = row["account"];
		m_strCreateTime = row["create_time"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_unLev = row["lev"];
		m_i64Id = row["id"];
		m_pCardMgr = new CCardMgr();
		if (!m_pCardMgr->Init(m_i64Id)) {
			cout << "�û�ӵ�еĿ��Ƽ���ʧ��" << endl;
			return false;
		}
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