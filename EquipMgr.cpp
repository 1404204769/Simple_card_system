#include "EquipMgr.h"
#include "CardMgr.h"
#include "User.h"
using namespace std;
CEquipMgr::CEquipMgr() {
	/*构造函数*/
	Log("调用了CEquipMgr构造函数\n");
}
CEquipMgr::~CEquipMgr() {
	/*析构函数*/
	Log("调用了CEquipMgr析构函数\n");

	Free();
	m_pUser = nullptr;
}

bool CEquipMgr::Init(CUser* pUser) {
	/*函数内部连接数据库获取数据*/
	try
	{
		if (!pUser)
		{
			cout << "构造装备管理器时发现用户不存在" << endl;
			return false;
		}

		m_pUser = pUser;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("CEquipMgr::Init()\n");
		if (!*pQuery) {
			Log("Query实例指针错误\n");//打印在控制台
			return false;
		}

		*pQuery << "select * from d_equip where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("CEquipMgr::Init()  从数据库查询用户装备数据失败\n");//打印在控制台
			return false;
		}
		Log("CEquipMgr::Init()  从数据库查询用户装备数据成功\n");
		while (mysqlpp::Row row = res.fetch_row()) {
			unique_ptr<CEquip> pEquip(new CEquip());
			if (!pEquip) {
				Log("CEquipMgr::Init()  装备实体化失败\n");//打印在控制台
				return false;
			}

			const CEquipType* pEquipType = g_EquipTypeMgr.Get(row["equip_type"]);
			if (!pEquipType) {
				cout << "用户所持有的装备类型非法,不予显示" << endl;
				return false;
			}
			if (!pEquip->CreateFromDB(row, pEquipType)) {
				Log("CEquipMgr::Init()  卡牌类型初始化失败\n");//打印在控制台
				return false;
			}
			const long long i64EquipId = pEquip->GetEquipId();
			if (!Add(pEquip.release())) {
				cout << "此装备（ID:" << i64EquipId << "）读入内存失败" << endl;
			}
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquipMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquipMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquipMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquipMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	m_bInit = true;
	return m_bInit;
}
bool CEquipMgr::IsInit() {
	/*返回是否初始化*/
	return m_bInit;
}

CEquip* CEquipMgr::Get(const long long int i64EquipId) {
	/*根据装备ID获取皮肤数据*/
	EquipMapIter iterById = m_map.find(i64EquipId);
	if (iterById == m_map.end()) {
		Log( "Do not Find EquipId:"+i64EquipId );
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
long long int CEquipMgr::GetWearCardId(const long long int i64EquipId) {
	/*判断某个装备是否被穿*/
	EquipCardMapIter iterById = m_mapEquipCard.find(i64EquipId);
	if (iterById == m_mapEquipCard.end()) {
		Log("此装备(ID:"+to_string(i64EquipId)+")目前还未被穿戴");
		return 0;
	}
	Log("此装备(ID:"+to_string( iterById->first )+")已被卡牌(ID: " + to_string( iterById->second )+")穿戴" );
	return iterById->second;
}

bool CEquipMgr::Add(CEquip* pEquip) {
	/*容器增加操作*/
	unique_ptr<CEquip>upEquip(pEquip);
	if (!upEquip) {
		cout << "传入的装备指针为空" << endl;
		return false;
	}

	const long long int i64EquipId = upEquip->GetEquipId();
	if (Get(i64EquipId)) {
		cout << "此装备ID已存在" << endl;
		return false;
	}

	m_map.insert({ i64EquipId ,upEquip.release() });//增加皮肤实体
	return true;
}
bool CEquipMgr::AddNew(const CEquipType* pEquipType) {
	/*增加一张指定类型的装备*/
	if (!m_pUser){
		cout << "此装备管理器与用户失联了" << endl;
		return false;
	}

	if (!pEquipType) {
		cout << "装备类型无效" << endl;
		return false;
	}
	unique_ptr<CEquip> pEquip(new CEquip());
	if (!pEquip) {
		cout << "Equip实体化失败" << endl;
		return false;
	}

	const long long int i64UserId = m_pUser->GetId();
	if (!pEquip->CreateNewEquip(i64UserId, pEquipType)) {
		cout << "装备数据初始化失败，增加装备失败" << endl;
		return false;
	}
	return Add(pEquip.release());
}
bool CEquipMgr::AddEquipCardMap(const long long int i64EquipId, const long long int i64CardId) {
	/*穿戴标记容器增加操作*/
	if (!Get(i64EquipId)) {
		return false;
	}
	const long long int i64WearCardId = GetWearCardId(i64EquipId);
	if (i64WearCardId!=0) {
		cout << "此装备已被卡牌（ID:" << i64WearCardId << "）穿戴" << endl;
		return false;
	}
	m_mapEquipCard.insert({ i64EquipId ,i64CardId });
	return true;
}

bool CEquipMgr::Del(const long long int i64EquipId) {
	/*根据装备ID来删除数据*/
	if (!m_pUser) {
		cout << "此装备管理器与用户失联了" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();

	unique_ptr<CEquip> upEquip(Get(i64EquipId));
	if (!upEquip) {
		cout << "删除失败，该玩家没有这张ID为：" << i64EquipId << " 的装备" << endl;
		return false;
	}

	const CEquipType* pEquipType = g_EquipTypeMgr.Get(upEquip->GetEquipType());
	const long long int i64CardId = GetWearCardId(i64EquipId);

	if (i64CardId) {
		cout << "此装备已被穿戴，正在脱下该装备" << endl;
		unsigned int unPos = 0;
		if (pEquipType)
			unPos = pEquipType->GetPos();
		CCard* pCard = CardMgr.Get(i64CardId);
		assert(pCard);//此时存在穿戴关系但是卡牌消失了，问题很严重
		if (pCard)
			pCard->Drop(unPos);
	}
	bool ret = upEquip->Delete();
	m_mapEquipCard.erase(i64EquipId);
	m_map.erase(i64EquipId);
	return ret;
}
bool CEquipMgr::DelExist(const long long int i64EquipId) {
	/*删除一张已有的装备*/
	return Del(i64EquipId);
}
bool CEquipMgr::DelEquipCardMap(const long long int i64EquipId) {
	/*穿戴标记容器删除操作*/
	if (!Get(i64EquipId)) {
		return false;
	}
	const long long int i64CardId=GetWearCardId(i64EquipId);
	if (i64CardId==0) {
		cout << "此装备未被穿戴" << endl;
		return false;
	}
	m_mapEquipCard.erase(i64EquipId);
	return true;
}

void CEquipMgr::PrintAll() {
	/*打印显示拥有的所有装备的数据*/
	for (auto& iter : m_map) {
		CEquip* pEquip = iter.second;
		if (!pEquip) {
			continue;
		}

		const unsigned int unEquipType = pEquip->GetEquipType();
		const CEquipType* pEquipType = g_EquipTypeMgr.Get(unEquipType);
		if (!pEquipType) {
			cout << "装备类型不存在" << endl;
			continue;
		}

		cout << "EquipID:" << pEquip->GetEquipId() << "\tUserId:" << pEquip->GetUserId()
			<< "\tEquipType:" << pEquipType->GetId() << "\tEquipName:" << pEquipType->GetName()
			<< "\tEquipAtk:" << pEquipType->GetAtk() << "\tEquipHp:" << pEquipType->GetHp()
			<< "\tEquipMp:" << pEquipType->GetMp() << endl;
	}
}

void CEquipMgr::Free(){
	/*在析构函数中调用，释放还在内存中的数据，防止内存泄漏以及数据丢失*/
	m_mapEquipCard.clear();
	for (auto& iter : m_map) {
		delete iter.second;//先释放内存
	}
	m_map.clear();
}