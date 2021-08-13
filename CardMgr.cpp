#include "CardMgr.h"
#include "User.h"
using namespace std;
CCardMgr::CCardMgr() {
	/*构造函数*/
	Log("调用了CCardMgr构造函数\n");
}
CCardMgr::~CCardMgr() {
	/*析构函数*/
	Log("调用了CCardMgr析构函数\n");

	Free();
	m_pUser = nullptr;
}
bool CCardMgr::Init(CUser* pUser) {
	/*函数内部连接数据库获取数据*/
	try
	{
		if (!pUser) 
		{
			cout << "构造卡牌管理器时发现用户不存在" << endl;
			return false;
		}

		m_pUser = pUser;
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log( "CCardMgr::Init()\n");
		if (!*pQuery) {
			Log("Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from d_card where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCardMgr::Init()  从数据库查询用户卡牌数据失败\n");//打印在控制台
			return false;
		}
		Log("CCardMgr::Init()  从数据库查询用户卡牌数据成功\n");

		while (row = res.fetch_row()) {
			unique_ptr<CCard> pCard(new CCard());
			if (!pCard) {
				Log("CCardMgr::Init()  卡牌实体化失败\n");//打印在控制台
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.Get(row["card_type"]);
			if (!pCardType) {
				cout << "用户所持有的卡牌类型非法,不予显示" << endl;
				continue;
			}
			if (!pCard->CreateFromDB(row, pCardType)) {
				Log("CCardMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
				return false;
			}
			m_mapByCardId[pCard->GetCardId()] = pCard.release();
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardMgr::Init()  Query error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardMgr::Init()  Conversion error: " +string( er.what())+ ",tretrieved data size: " +to_string( er.retrieved )+ ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCardMgr::Init()  Error: " +string( er.what())+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardMgr::Init()  Error: " +string( er.what())+ "\n");
		return false;
	}
	return true;
}
CCard* CCardMgr::Get(const long long int _i64CardId) {
	/*根据卡牌名称获取卡牌数据*/
	CardMapIter iterById = m_mapByCardId.find(_i64CardId);
	if (iterById == m_mapByCardId.end()) {
		cout << "Do not Find CardId:" << _i64CardId << endl;
		return nullptr;
	}
	return iterById->second;
}
void CCardMgr::PrintAll() {
	/*打印显示拥有的所有卡牌的数据*/
	CardMapIter iterById = m_mapByCardId.begin();
	while (iterById != m_mapByCardId.end()) {
		CCard* pCard = iterById->second;
		iterById++;
		if (!pCard) {
			continue;
		}
		const CCardType& CardType = pCard->GetCardTypeData();
		const CCardLevAttrType& CardLevAttrType = pCard->GetCardLevAttrTypeData();
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tName:" << pCard->GetName() << "\tLev:" << pCard->GetLev()
			<< "\tExp:" << pCard->GetExp() 
			<< "\tHp:" << CardType.GetHp()+CardLevAttrType.GetHp() 
			<< "\tMp:" << CardType.GetMp()+CardLevAttrType.GetMp()
			<< "\tAtk:" << CardType.GetAtk()+CardLevAttrType.GetAtk()<<endl;
	}
}
bool CCardMgr::Add(const CCardType* pCardType) {
	/*增加一张指定类型的卡牌*/
	if (!m_pUser) 
	{
		cout << "此卡牌管理器与用户失联了" << endl;
		return false;
	}

	if (!pCardType) {
		cout << "卡牌类型无效" << endl;
		return false;
	}

	unique_ptr<CCard> pCard(new CCard());
	if (!pCard) {
		cout << "Card实体化失败" << endl;
		return false;
	}

	if (!pCard->CreateNewCard(m_pUser->GetId(),pCardType)) {
		cout << "卡牌数据初始化失败，增加卡牌失败" << endl;
		return false;
	}

	m_mapByCardId[pCard->GetCardId()] = pCard.release();
	return true;
}
bool CCardMgr::Del(long long int i64CardId) {
	/*根据玩家卡牌名称来删除数据*/
	if (i64CardId <= 0) {
		cout << "卡牌ID不合法" << endl;
		return false;
	}

	CCard* pCard = m_mapByCardId[i64CardId];
	m_mapByCardId[i64CardId] = nullptr;

	if (!pCard) {
		cout << "删除失败，该玩家没有这张叫做（"<< i64CardId <<"）的卡牌" << endl;
		return false;
	}

	m_mapByCardId.erase(i64CardId);
	if(!pCard->Delete())
	{
		delete pCard;
		return false;
	}
	delete pCard;
	return true;
}
bool CCardMgr::DelAll() {
	/*删除所有的卡牌*/
	bool bRet = true;
	CardMapIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		CCard* pCard = iterByName->second;
		iterByName->second = nullptr;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}	

		if (!pCard->Delete()) {
			cout << "卡牌删除失败" << endl;
			bRet = false;
		}
		delete pCard;
	}
	m_mapByCardId.clear();
	return bRet;
}
void CCardMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	CardMapIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		delete iterByName->second;//先释放内存
		iterByName->second = nullptr;//置空
		iterByName++;
	}
	m_mapByCardId.clear();
}

