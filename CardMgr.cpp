#include "CardMgr.h"
using namespace std;
CCardMgr::CCardMgr() {
	/*构造函数*/
	stringstream strIn;
	strIn << "调用了CCardMgr构造函数\n";
	OutputDebugPrintf(strIn.str().c_str());
}
CCardMgr::~CCardMgr() {
	/*析构函数*/
	stringstream strIn;
	strIn << "调用了CCardMgr析构函数\n";
	OutputDebugPrintf(strIn.str().c_str());
	Free();
}
bool CCardMgr::Init(long long int _i64UserId) {
	/*函数内部连接数据库获取数据*/
	try
	{
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.getQuery();
		stringstream strIn;
		strIn << "CCardMgr::Init()\n";
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		*pQuery << "select * from d_card where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = _i64UserId;
		bool bRet = g_DB.Search(res, *pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		delete pQuery;
		pQuery = nullptr;
		if (!bRet) {
			strIn << "从数据库查询用户卡牌数据失败\n";
			OutputDebugPrintf(strIn.str().c_str());//打印在控制台
			return false;
		}
		strIn << "从数据库查询用户卡牌数据成功\n";
		while (row = res.fetch_row()) {
			CCard* pCard = new CCard();
			if (!pCard) {
				strIn << "卡牌实体化失败\n";
				OutputDebugPrintf(strIn.str().c_str());//打印在控制台
				delete pCard;
				pCard = nullptr;
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(row["card_type"]);
			if (!pCardType) {
				cout << "用户所持有的卡牌类型非法,不予显示" << endl;
				delete pCardType;
				pCardType = nullptr;
				continue;
			}
			if (!pCard->Init(row["id"], row["user_id"], row["card_type"], string(row["name"]), row["exp"], row["lev"], pCardType)) {
				strIn << "卡牌类型初始化失败\n";
				OutputDebugPrintf(strIn.str().c_str());//打印在控制台
				delete pCard;
				pCard = nullptr;
				return false;
			}
			pCard->SetMark(4);
			m_mapByCardName[pCard->GetName()] = pCard;
		}
		OutputDebugPrintf(strIn.str().c_str());//打印在控制台
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nQuery error: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		OutputDebugPrintf(strIn.str().c_str());
		return false;
	}
	return true;
}
CCard* CCardMgr::GetCardByCardName(std::string _strName) {
	/*根据卡牌名称获取卡牌数据*/
	MapByCardNameIter iterByName = m_mapByCardName.find(_strName);
	if (iterByName == m_mapByCardName.end()) {
		cout << "Do not Find CardName:" << _strName << endl;
		return nullptr;
	}
	return iterByName->second;
}
void CCardMgr::PrintAllCard() {
	/*打印显示拥有的所有卡牌的数据*/
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			continue;
		}
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tCardType:" << pCard->GetCardType() << "\tName:" << pCard->GetName() << "\tExp:" << pCard->GetExp() << "\tLev:" << pCard->GetLev() <<"\tMark:"<<pCard->GetMark() << endl;
	}
}
bool CCardMgr::AddCard(long long int _i64UserId, const CCardType* _pCardType) {
	/*增加一张指定类型的卡牌*/
	if (!_pCardType) {
		cout << "卡牌类型无效" << endl;
		return false;
	}
	CCard* pCard = new CCard();
	if (!pCard) {
		delete pCard;
		cout << "Card实体化失败" << endl;
		return false;
	}
	string strName;
	cout << "请输入卡牌的昵称(注意：不能与已有卡牌重名)" << endl;
	getline(cin, strName);
	while (GetCardByCardName(strName)) {
		cout << "卡牌名称()已存在，请重新输入卡牌的昵称(注意：不能与已有卡牌重名，不能含有空格)" << endl;
		getline(cin, strName);
	}
	if (!pCard->Init(0, _i64UserId, _pCardType->getCardType(), strName, 0, 1, _pCardType)) {
		cout << "卡牌数据初始化失败，增加卡牌失败" << endl;
		delete pCard;
		pCard = nullptr;
		return false;
	}
	pCard->SetMark(1);
	m_mapByCardName[pCard->GetName()] = pCard;
	return true;
}
bool CCardMgr::DelCardByName(std::string _strName) {
	/*根据玩家卡牌名称来删除数据*/
	CCard* pCard = GetCardByCardName(_strName);
	if (!pCard) {
		cout << "删除失败，该玩家没有这张叫做（"<<_strName<<"）的卡牌" << endl;
		return false;
	}
	pCard->SetMark(2);
	return true;
}
bool CCardMgr::DelAllCard() {
	/*删除所有的卡牌*/
	bool bRet = true;
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}
		pCard->SetMark(2);
	}
	return bRet;
}
void CCardMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		delete iterByName->second;//先释放内存
		iterByName->second = nullptr;//置空
		iterByName++;
	}
	m_mapByCardName.clear();
}