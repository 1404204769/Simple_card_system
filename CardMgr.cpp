#include "CardMgr.h"
using namespace std;
CCardMgr::CCardMgr() {
	/*构造函数*/
	stringstream strIn;
	strIn << "调用了CCardMgr构造函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCardMgr::~CCardMgr() {
	/*析构函数*/
	stringstream strIn;
	strIn << "调用了CCardMgr析构函数\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	Free();
}
bool CCardMgr::Init(long long int _i64UserId) {
	/*函数内部连接数据库获取数据*/
	try
	{
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		stringstream strIn;
		strIn << "CCardMgr::Init()\n";
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//打印在控制台
			return false;
		}
		*pQuery << "select * from d_card where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = _i64UserId;
		strIn << "Query:" << pQuery->str() << "\n";
		if (!g_DB.Search(res, *pQuery)) {
			strIn << "从数据库查询用户卡牌数据失败\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//打印在控制台
			return false;
		}
		strIn << "从数据库查询用户卡牌数据成功\n";
		while (row = res.fetch_row()) {
			CCard* pCard = new CCard();
			if (!pCard) {
				strIn << "卡牌实体化失败\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//打印在控制台
				delete pCard;
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(row["card_type"]);
			if (!pCardType) {
				cout << "用户所持有的卡牌类型非法,不予显示" << endl;
				delete pCard;
				continue;
			}
			if (!pCard->Init(row["id"], row["user_id"], row["card_type"], string(row["name"]), row["exp"], row["lev"], pCardType)) {
				strIn << "卡牌类型初始化失败\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//打印在控制台
				delete pCard;
				return false;
			}
			m_mapByCardId[pCard->GetCardId()] = pCard;
		}
		m_vecDel.clear();
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());//打印在控制台
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	return true;
}
CCard* CCardMgr::GetCardByCardId(long long int _i64CardId) {
	/*根据卡牌名称获取卡牌数据*/
	MapByCardIdIter iterByName = m_mapByCardId.find(_i64CardId);
	if (iterByName == m_mapByCardId.end()) {
		cout << "Do not Find CardId:" << _i64CardId << endl;
		return nullptr;
	}
	return iterByName->second;
}
void CCardMgr::PrintAllCard() {
	/*打印显示拥有的所有卡牌的数据*/
	MapByCardIdIter iterById = m_mapByCardId.begin();
	while (iterById != m_mapByCardId.end()) {
		CCard* pCard = iterById->second;
		iterById++;
		if (!pCard) {
			continue;
		}
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tCardType:" << pCard->GetCardType() << "\tName:" << pCard->GetName() << "\tExp:" << pCard->GetExp() << "\tLev:" << pCard->GetLev() <<"\tMark:"<<pCard->GetMark() << endl;
		const CCardType* pCardType = pCard->GetCardTypeData();
		if (!pCardType) {
			cout << "无法获取对应类型" << endl;
			continue;
		}
		cout << "\t(ID:" << pCardType->GetId() << "\tCardTypeId:" << pCardType->GetCardType() << "\tCardName:" << pCardType->GetName() << "\tHp:" << pCardType->GetHp() << "\tMp:" << pCardType->GetMp() << "\tAtk:" << pCardType->GetAtk() <<")" << endl;
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
	long long int i64CardId = 0;
	if (!Insert(i64CardId, _i64UserId, _pCardType->GetCardType(), "", 0, 1)) {
		cout << "用户新增卡牌插入数据库失败" << endl;
		return false;
	}
	if (!pCard->Init(i64CardId, _i64UserId, _pCardType->GetCardType(), "", 0, 1,_pCardType)) {
		cout << "卡牌数据初始化失败，增加卡牌失败" << endl;
		delete pCard;
		return false;
	}
	m_mapByCardId[pCard->GetCardId()] = pCard;
	return true;
}
bool CCardMgr::DelCardById(long long int _i64CardId) {
	/*根据玩家卡牌名称来删除数据*/
	CCard* pCard = m_mapByCardId[_i64CardId];
	if (!pCard) {
		cout << "删除失败，该玩家没有这张叫做（"<<_i64CardId<<"）的卡牌" << endl;
		return false;
	}
	m_mapByCardId.erase(_i64CardId);
	m_vecDel.push_back(pCard);
	return true;
}
bool CCardMgr::DelAllCard() {
	/*删除所有的卡牌*/
	bool bRet = true;
	MapByCardIdIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}
		m_vecDel.push_back(pCard);
	}
	m_mapByCardId.clear();
	return bRet;
}
void CCardMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	MapByCardIdIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		delete iterByName->second;//先释放内存
		iterByName->second = nullptr;//置空
		iterByName++;
	}
	m_mapByCardId.clear();
	vector<CCard*>::iterator VecIter = m_vecDel.begin();
	while (VecIter != m_vecDel.end()) {
		CCard* pCard = *VecIter++;
		if(!pCard)
			continue;
		Delete(*pCard);
	}
	m_vecDel.clear();
}


/*数据库相关接口*/
bool CCardMgr::Insert(long long int& _i64CardId, const long long int _i64UserId, const unsigned int _unCardType, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev){
	/*将Card数据插入数据库*/
	/*在数据库中插入新的User数据*/
	try
	{
		stringstream strIn;
		strIn << "CCard::Insert()\n";
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		*pQuery << "insert into d_card values(0,%0q:user_id, %1q:card_type,%2q:name,%3q:exp,%4q:lev)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = _i64UserId;
		pQuery->template_defaults["card_type"] = _unCardType;
		pQuery->template_defaults["name"] = _strName.c_str();
		pQuery->template_defaults["exp"] = _i64Exp;
		pQuery->template_defaults["lev"] = _unLev;
		bool bRet = g_DB.Insert(*pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		if (!bRet) {
			strIn << "往数据库插入用户新卡牌数据失败\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn << "往数据库插入用户新卡牌数据成功\n";
		pQuery->reset();
		*pQuery << "select @@IDENTITY as ID";
		pQuery->parse();
		mysqlpp::UseQueryResult res;
		if (!g_DB.Search(res, *pQuery)) {
			cout << "获取系统分配的CardId失败" << endl;
			return false;
		}
		mysqlpp::Row row=res.fetch_row();
		if (!row) {
			cout << "没有获取到系统分配的CardId" << endl;
			return false;
		}
		_i64CardId = row["ID"];
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
bool CCardMgr::Delete(CCard& Card) {
	/*将Card数据从数据库删除*/
	try
	{
		stringstream strIn;
		strIn << "CCard::Delete()\n";
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			strIn << "Query实例指针错误\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		*pQuery << "delete from d_card where id = %0q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["CardId"] = Card.GetCardId();
		bool bRet = g_DB.Delete(*pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		if (!bRet) {
			strIn << "从数据库删除用户卡牌数据失败\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn << "从数据库删除用户卡牌数据成功\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}