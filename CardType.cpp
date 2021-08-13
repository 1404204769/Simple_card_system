#include "CardType.h"
using namespace std;
CCardType::CCardType() {
	/*构造函数*/
	Log("调用了CCardType构造函数\n");
}
CCardType::~CCardType() {
	/*析构函数*/
	Log("调用了CCardType析构函数\n");
}
bool CCardType::Init(const mysqlpp::Row& row) {
	/*根据获取的数据库数据初始化静态卡牌数据*/
	/*根据查询获取的mysqlpp::Row对User对象初始化*/
	try
	{
		if (!row) {
			Log("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/

		m_unId = row["id"];
		m_unCardType = row["card_type"];
		m_strName = row["name"];
		m_i64Hp = row["hp"];
		m_i64Mp = row["mp"];
		m_i64Atk = row["atk"];
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardType::Init()  Query error: " +string( er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardType::Init()  Conversion error: "+ string( er.what())+ ",tretrieved data size: "+to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)+ "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCardType::Init() Error: " + string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardType::Init()  Error: " +string(er.what()) + "\n");
		return false;
	}
	return true;
}
const std::string& CCardType::GetName() const {
	/*获取m_strName*/
	return m_strName;
}
unsigned int CCardType::GetId() const {
	/*获取m_unId*/
	return m_unId;
}
unsigned int CCardType::GetCardType() const {
	/*获取m_unCardType*/
	return m_unCardType;
}
long long int CCardType::GetHp() const {
	/*获取m_i64Hp*/
	return m_i64Hp;
}
long long int CCardType::GetMp() const {
	/*获取m_i64Mp*/
	return m_i64Mp;
}
long long int CCardType::GetAtk() const {
	/*获取m_i64Atk*/
	return m_i64Atk;
}