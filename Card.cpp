#include "Card.h"
using namespace std;
CCard::CCard() {
	/*构造函数*/
	Log("调用了CCard构造函数\n");
}
CCard::~CCard(){
	/*析构函数*/
	Log("调用了CCard析构函数\n");

	if (!Update())
		Log("用户卡牌数据保存失败\n");
	else
		Log("用户卡牌数据保存成功\n");
}

bool CCard::CreateNewCard(const long long int i64UserId, const CCardType* pCardType) {
	/*构造一个新的卡牌*/
	if (!pCardType)
	{
		cout << "卡牌类型无效，新建卡牌失败" << endl;
		return false;
	}

	if (i64UserId<=0) {
		cout << "用户ID不合法" << endl;
		return false;
	}

	if (!SetLevel(1)) {/*初始等级为0*/
		cout << "卡牌等级无效" << endl;
		return false;
	}
	if (!SetRank(0)) {
		/*初始阶级为0*/
		cout << "卡牌阶级无效" << endl;
		return false;
	}
	m_i64UserId = i64UserId;
	m_unCardType = pCardType->GetCardType();
	m_strName = pCardType->GetName();
	m_i64Exp = 0;
	m_pCardType = pCardType;

	if (!Insert(m_i64CardId)) {
		cout << "新卡牌插入数据库失败" << endl;
		return false;
	}
	return true;
}
bool CCard::CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType) {
	/*从数据库中加载卡牌*/
	try
	{
		if (!row) 
		{
			Log("初始化卡牌失败\n");//打印在控制台
			return false;
		}/*否则说明找到对应数据，将其映射到Card对象上*/

		if (!pCardType)
		{
			cout << "卡牌类型无效，新建卡牌失败" << endl;
			return false;
		}

		m_unCardType = row["card_type"];
		if (m_unCardType != pCardType->GetCardType())
		{
			cout << "卡牌类型不匹配，新建卡牌失败" << endl;
			return false;
		}

		if (!SetLevel(row["lev"])) {
			cout << "卡牌等级无效" << endl;
			return false;
		}
		if (!SetRank(row["rank_lev"])) {
			cout << "卡牌阶级无效" << endl;
			return false;
		}
		m_i64CardId = row["id"];
		m_i64UserId = row["user_id"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_pCardType = pCardType;

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::CreateFromDB()  Conversion error: " +string( er.what()) + ",tretrieved data size: " + to_string( er.retrieved)+ ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

const std::string& CCard::GetName() const{
	/*获取m_strName*/
	return m_strName;
}
unsigned int CCard::GetLev() const {
	/*获取m_unLev*/
	return m_unLev;
}
unsigned int CCard::GetCardRankLev() const {
	/*获取m_unRankLev*/
	return m_unRankLev;
}
unsigned int CCard::GetCardType() const{
	/*获取m_unCardType*/
	return m_unCardType;
}
long long int CCard::GetCardId() const{
	/*获取m_i64CardId*/
	return m_i64CardId;
}
long long int CCard::GetUserId() const{
	/*获取m_i64UserId*/
	return m_i64UserId;
}
long long int CCard::GetExp() const{
	/*获取m_i64Exp*/
	return m_i64Exp;
}
const CCardType& CCard::GetCardTypeData() const{
	/*获取对应的卡牌类型详细数据*/
	assert(m_pCardType);
	return *m_pCardType;
}
const CCardLevAttrType& CCard::GetCardLevAttrTypeData() const {
	/*获取对应卡牌等级的加成数据*/
	assert(m_pCardLevAttrType);

	return *m_pCardLevAttrType;
}
const CCardRankType& CCard::GetCardRankTypeData() const {
	/*获取对应卡牌阶级的加成数据*/
	assert(m_pCardRankType);

	return *m_pCardRankType;
}
bool CCard::LevelUp(unsigned int unLev) {
	/*提升卡牌等级，更换加成数据*/
	return SetLevel(unLev + m_unLev);
}
bool CCard::RankUp() {
	/*提升卡牌阶级，更换加成数据*/
	return SetRank(m_unRankLev+1);
}
bool CCard::SetLevel(unsigned int unLev) {
	if (unLev <= 0)
		return false;

	const CCardLevAttrType* pCardLevAttrType = g_CardLevAttrTypeMgr.Get(unLev);
	if (!pCardLevAttrType) {
		cout << "卡牌等级超出限制" << endl;
		return false;
	}

	m_pCardLevAttrType = pCardLevAttrType;
	m_unLev = unLev;
	return true;
}
bool CCard::SetRank(unsigned int unRank) {
	/*设置Rank等级*/
	if (unRank <= 0)
		return false;

	const CCardRankType* pCardRankType = g_CardRankTypeMgr.Get(unRank);
	if (!pCardRankType) {
		cout << "卡牌阶级超出限制" << endl;
		return false;
	}

	m_pCardRankType = pCardRankType;
	m_unRankLev = unRank;
	return true;
}

/*
* 以下是数据库层相关接口
*/


bool CCard::Insert(long long int& i64CardId_Out) {
	/*将Card数据插入数据库*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Insert()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "insert into d_card values(0,%0q:user_id, %1q:card_type,%2q:name,%3q:exp,%4q:lev，%5q:rank_lev)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["card_type"] = m_unCardType;
		pQuery->template_defaults["name"] = m_strName.c_str();
		pQuery->template_defaults["exp"] = m_i64Exp;
		pQuery->template_defaults["lev"] = m_unLev;
		pQuery->template_defaults["rank_lev"] = m_unRankLev;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CCard::Insert()  往数据库插入用户新卡牌数据失败\n");
			return false;
		}
		Log("CCard::Insert()  往数据库插入用户新卡牌数据成功\n");

		pQuery->reset();
		*pQuery << "select @@IDENTITY as ID";
		pQuery->parse();

		mysqlpp::UseQueryResult res;
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCard::Insert()  获取系统分配的CardId失败\n");
			return false;
		}
		mysqlpp::Row row = res.fetch_row();
		if (!row) {
			Log("CCard::Insert()  没有获取到系统分配的CardId\n");
			return false;
		}
		i64CardId_Out = row["ID"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Insert()  Query error: " + string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Insert()  Conversion error: " +string( er.what()) + ",tretrieved data size: " +to_string( er.retrieved) + ", actual size: " +to_string(er.actual_size) +"\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Insert()  Error: " +string( er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CCard::Delete() {
	/*将Card数据从数据库删除*/
	try
	{
		Log("CCard::Delete()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Delete()  Query实例指针错误\n");
			return false;
		}

		*pQuery << "delete from d_card where id = %0q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["CardId"] =m_i64CardId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CCard::Delete()  从数据库删除用户卡牌数据失败\n");
			return false;
		}
		Log("CCard::Delete()  从数据库删除用户卡牌数据成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Delete()  Query error: " +string( er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Delete()  Conversion error: "+ string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size)+ "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Delete()  Error: " +string( er.what() )+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Delete()  Error: "+ string( er.what())+ "\n");
		return false;
	}
	return true;
}
bool CCard::Update() {
	/*更新数据库*/
	try
	{
		Log("CCard::Update()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Update()  Query对象不存在，无法更新数据\n");
			return false;
		}

		*pQuery << "update `d_card` set user_id=%0q:UserId,`name`=%1q:Name,exp=%2q:Exp,lev=%3q:Lev where id = %4q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_i64UserId;
		pQuery->template_defaults["Name"] = m_strName.c_str();
		pQuery->template_defaults["Exp"] = m_i64Exp;
		pQuery->template_defaults["Lev"] = m_unLev;
		pQuery->template_defaults["CardId"] = m_i64CardId;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Update(*pQuery)) {
			Log("CCard::Update()  向数据库更新用户卡牌失败\n");
			return false;
		}

		Log("CCard::Update()  向数据库更新用户卡牌成功\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Update()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Update()  Conversion error: " +string( er.what() )+",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Update()  Error: " +string( er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Update()  Error: " +string( er.what()) + "\n");
		return false;
	}
	return true;
}