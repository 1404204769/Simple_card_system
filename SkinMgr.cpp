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
		CCardMgr& CardMgr = m_pUser->GetCardMgr();
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

		while (mysqlpp::Row row = res.fetch_row()) {
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

			long long int i64CardId = 0;
			if (pSkin->IsWear()&&CardMgr.Get(pSkin->GetCardId())) {
				i64CardId = pSkin->GetCardId();
			}

			const long long int i64SkinId = pSkin->GetSkinId();
			m_mapBySkinId.insert({ i64SkinId,pSkin.release() });
			if (i64CardId != 0) {
				m_mapByCardId.insert({ i64CardId ,m_mapBySkinId[i64SkinId] });
			}
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
	assert(iterById->second);

	return iterById->second;
}
CSkin* CSkinMgr::GetWearing(const long long int i64CardId) {
	/*根据皮肤ID获取皮肤数据*/
	CardSkinMapIter iterById = m_mapByCardId.find(i64CardId);
	if (iterById == m_mapByCardId.end()) {
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
bool CSkinMgr::AddNew( const CSkinType* pSkinType) {
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
	
	const long long int i64UserId = m_pUser->GetId();
	if (!pSkin->CreateNewSkin(i64UserId,pSkinType)) {
		cout << "皮肤数据初始化失败，增加皮肤失败" << endl;
		return false;
	}

	const long long int i64SkinId = pSkin->GetSkinId();
	m_mapBySkinId.insert({ i64SkinId ,pSkin.release() });
	return true;
}
bool CSkinMgr::Del(long long int i64SkinId) {
	/*根据皮肤Id删除皮肤*/
	if (i64SkinId <= 0) {
		cout << "皮肤ID不合法" << endl;
		return false;
	}
	CSkin* pSkin = nullptr;
	if(m_mapBySkinId.count(i64SkinId))
		pSkin = m_mapBySkinId[i64SkinId];

	if (!pSkin) {
		cout << "删除失败，该玩家没有这张ID为：" << i64SkinId << " 的皮肤" << endl;
		return false;
	}
	const long long int i64CardId = pSkin->GetCardId();
	if (i64CardId != 0) {
		m_mapByCardId.erase(i64CardId);
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
void CSkinMgr::DeBug_DelAll() {
	/*删除所有的皮肤*/
	for(auto &iter:m_mapBySkinId) {
		CSkin* pSkin = iter.second;
		if (!pSkin) {
			continue;
		}
		if (!pSkin->Delete()) {
			cout << "卡牌删除失败" << endl;
		}
		delete pSkin;
	}
	m_mapByCardId.clear();
	m_mapBySkinId.clear();
	return ;
}
void CSkinMgr::PrintAll() {
	/*打印显示拥有的所有皮肤牌的数据*/
	for (auto& iter : m_mapBySkinId) {
		CSkin* pSkin = iter.second;
		if (!pSkin) {
			continue;
		}
		const unsigned int unSkinType = pSkin->GetSkinType();
		const CSkinType* SkinType = g_SkinTypeMgr.Get(unSkinType);
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
	if (!pSkin->Wear(*pCard)) {
		return false;
	}
	m_mapByCardId.insert({ i64CardId,pSkin });
	return true;
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
	if (!pSkin->Drop(*pCard)) {
		return false;
	}
	m_mapByCardId.erase(i64CardId);
	return true;
}
void CSkinMgr::Free() {
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	for (auto& iter:m_mapBySkinId) {
		delete iter.second;//先释放内存
	}
	m_mapBySkinId.clear();
	m_mapByCardId.clear();
}
