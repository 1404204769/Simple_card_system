#include "SkinMgr.h"
#include "User.h"
#include "CardMgr.h"
using namespace std;
CSkinMgr::CSkinMgr() {
	/*构造函数*/
	Log("调用了CSkinMgr构造函数\n");
}
CSkinMgr::~CSkinMgr() {
	/*析构函数*/
	Log("调用了CSkinMgr析构函数\n");

	Free();
	m_pUser = nullptr;
}

bool CSkinMgr::Init(CUser* pUser) {
	/*函数内部连接数据库获取数据*/
	try
	{
		if (!pUser)
		{
			cout << "构造皮肤管理器时发现用户不存在" << endl;
			return false;
		}

		m_pUser = pUser;
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("SkinMgr::Init()\n");
		if (!*pQuery) {
			Log("Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from d_skin where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("SkinMgr::Init()  从数据库查询用户皮肤数据失败\n");//打印在控制台
			return false;
		}
		Log("CSkinMgr::Init()  从数据库查询用户皮肤数据成功\n");

		while (row = res.fetch_row()) {
			unique_ptr<CSkin> pSkin(new CSkin());
			if (!pSkin) {
				Log("CSkinMgr::Init()  皮肤实体化失败\n");//打印在控制台
				return false;
			}
			const CSkinType* pSkinType = g_SkinTypeMgr.Get(row["skin_type"]);
			if (!pSkinType) {
				cout << "用户所持有的皮肤类型非法,不予显示" << endl;
				continue;
			}
			if (!pSkin->CreateFromDB(row, pSkinType)) {
				Log("CSkinMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
				return false;
			}
			m_mapBySkinId[pSkin->GetSkinId()] = pSkin.release();
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkinMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkinMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkinMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkinMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

CSkin* CSkinMgr::Get(const long long int i64SkinId) {
	/*根据卡牌名称获取卡牌数据*/
	SkinMapIter iterById = m_mapBySkinId.find(i64SkinId);
	if (iterById == m_mapBySkinId.end()) {
		cout << "Do not Find SkinId:" << i64SkinId << endl;
		return nullptr;
	}
	return iterById->second;
}

bool CSkinMgr::Add(const long long int i64UserId, const CSkinType* pSkinType) {
	/*获得一个新皮肤*/
	if (!m_pUser)
	{
		cout << "此皮肤管理器与用户失联了" << endl;
		return false;
	}

	if (!pSkinType) {
		cout << "皮肤类型无效" << endl;
		return false;
	}

	unique_ptr<CSkin> pSkin(new CSkin());
	if (!pSkin) {
		cout << "Skin实体化失败" << endl;
		return false;
	}

	if (!pSkin->CreateNewSkin(i64UserId,pSkinType)) {
		cout << "皮肤数据初始化失败，增加皮肤失败" << endl;
		return false;
	}
	m_mapBySkinId[pSkin->GetSkinId()] = pSkin.release();
	return true;
}
bool CSkinMgr::Del(long long int i64SkinId) {
	/*根据皮肤Id删除皮肤*/
	if (i64SkinId <= 0) {
		cout << "皮肤ID不合法" << endl;
		return false;
	}

	CSkin* pSkin = m_mapBySkinId[i64SkinId];
	m_mapBySkinId[i64SkinId] = nullptr;

	if (!pSkin) {
		cout << "删除失败，该玩家没有这张ID为：" << i64SkinId << " 的皮肤" << endl;
		return false;
	}

	m_mapBySkinId.erase(i64SkinId);
	if (!pSkin->Delete())
	{
		delete pSkin;
		return false;
	}
	delete pSkin;
	return true;
}
bool CSkinMgr::DelAll() {
	/*删除所有的皮肤*/
	bool bRet = true;
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		CSkin* pSkin = iterById->second;
		iterById->second = nullptr;
		iterById++;
		if (!pSkin) {
			bRet = false;
			continue;
		}

		if (!pSkin->Delete()) {
			cout << "卡牌删除失败" << endl;
			bRet = false;
		}
		delete pSkin;
	}
	m_mapBySkinId.clear();
	return bRet;
}
void CSkinMgr::PrintAll() {
	/*打印显示拥有的所有皮肤牌的数据*/
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		CSkin* pSkin = iterById->second;
		iterById++;
		if (!pSkin) {
			continue;
		}
		const CSkinType* SkinType = g_SkinTypeMgr.Get(pSkin->GetSkinType());
		if (!SkinType) {
			cout << "皮肤类型不存在" << endl;
			continue;
		}
		cout << "SkinID:" << pSkin->GetSkinId() << "\tUserId:" << pSkin->GetUserId() << "\tWear:" << pSkin->GetCardId() 
			<< "\tSkinType:" << SkinType->GetId() << "\tSkinName:" << SkinType->GetName() 
			<< "\tSkinAtk:" << SkinType->GetAtk() << "\tSkinHp:" << SkinType->GetHp() << "\tSkinMp:" << SkinType->GetMp() << endl;
	}
}
bool CSkinMgr::Wear(const long long int i64CardId, const long long int i64SkinId) {
	/*指定某张卡牌穿上某个皮肤*/
	if (!m_pUser) {
		cout << "用户与皮肤管理器失联了" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "没有找到要穿上皮肤的卡牌" << endl;
		return false;
	}
	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		cout << "没有找到要穿戴的皮肤" << endl;
		return false;
	}
	if (pSkin->IsWear()) {
		cout << "要穿戴的皮肤已经被卡牌("<<pSkin->GetCardId() << ")穿上了" << endl;
		return false;
	}
	return pSkin->Wear(*pCard);
}
bool CSkinMgr::Drop(const long long int i64CardId, const long long int i64SkinId) {
	/*指定某张卡牌脱下某个皮肤*/
	if (!m_pUser) {
		cout << "用户与皮肤管理器失联了" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "没有找到要脱下皮肤的卡牌" << endl;
		return false;
	}
	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		cout << "没有找到要脱下的皮肤" << endl;
		return false;
	}
	if (!pSkin->IsWear()) {
		cout << "要脱下的皮肤本就无人使用" << endl;
		return false;
	}
	return pSkin->Drop(*pCard);
}
void CSkinMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		delete iterById->second;//先释放内存
		iterById->second = nullptr;//置空
		iterById++;
	}
	m_mapBySkinId.clear();
}
