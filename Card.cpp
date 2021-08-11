#include "Card.h"
using namespace std;
CCard::CCard() {
	/*构造函数*/
	stringstream strIn;
	strIn << "调用了CCard构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCard::~CCard(){
	/*析构函数*/
	stringstream strIn;
	strIn << "调用了CCard析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	if (m_bMark)Update();
	m_pCardType = nullptr;
}
bool CCard::Init(const long long int _i64CardId, const long long int _i64UserId, const unsigned int _unCardType, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev, const CCardType* _pCardType) {
	/*根据获取的数据库数据初始化静态卡牌数据*/
	m_i64CardId = _i64CardId;
	m_i64UserId = _i64UserId;
	m_unCardType = _unCardType;
	m_strName = _strName;
	m_i64Exp = _i64Exp;
	m_unLev = _unLev;
	m_pCardType = _pCardType;
	m_bMark = false;
	if (m_i64UserId == 0 || !m_pCardType)
		return false;
	return true;
}
std::string& CCard::GetName() {
	/*获取m_strName*/
	return m_strName;
}
unsigned int CCard::GetLev() {
	/*获取m_unLev*/
	return m_unLev;
}
unsigned int CCard::GetCardType() {
	/*获取m_unCardType*/
	return m_unCardType;
}
bool CCard::GetMark() {
	/*获取m_unMark*/
	return m_bMark;
}
long long int CCard::GetCardId() {
	/*获取m_i64CardId*/
	return m_i64CardId;
}
long long int CCard::GetUserId() {
	/*获取m_i64UserId*/
	return m_i64UserId;
}
long long int CCard::GetExp() {
	/*获取m_i64Exp*/
	return m_i64Exp;
}
const CCardType* CCard::GetCardTypeData() {
	/*获取对应的卡牌类型详细数据*/
	return m_pCardType;
}
void CCard::SetMark(bool _bMark) {
	/*设置栈状态*/
	m_bMark = _bMark;
}

/*
* 以下是数据库层相关接口
*/

bool CCard::Update() {
	/*更新数据库*/
	try
	{
		stringstream strIn;
		strIn << "CCard::Update()\n";
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			delete pQuery;
			strIn << "Query对象不存在，无法更新数据\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		*pQuery << "update `d_card` set user_id=%0q:UserId,`name`=%1q:Name,exp=%2q:Exp,lev=%3q:Lev where id = %4q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_i64UserId;
		pQuery->template_defaults["Name"] = m_strName.c_str();
		pQuery->template_defaults["Exp"] = m_i64Exp;
		pQuery->template_defaults["Lev"] = m_unLev;
		pQuery->template_defaults["CardId"] = m_i64CardId;
		strIn << "Query:" << pQuery->str() << "\n";
		bool bRet = g_DB.Update(*pQuery);
		delete pQuery;
		if (!bRet) {
			strIn << "向数据库更新用户卡牌失败\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());;
			return false;
		}
		strIn << "向数据库更新用户卡牌成功\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCard::Update()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCard::Update()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCard::Update()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCard::Update()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}