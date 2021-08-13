#include "CardLevAttrType.h"
using namespace std;
CCardLevAttrType::CCardLevAttrType() {
	/*构造函数*/
	Log("调用了CCardLevAttrType构造函数\n");
}
CCardLevAttrType::~CCardLevAttrType() {
	/*析构函数*/
	Log("调用了CCardLevAttrType析构函数\n");
}
bool CCardLevAttrType::Init(const mysqlpp::Row& row) {
	/*根据获取的数据库数据初始化静态卡牌数据*/
	/*根据查询获取的mysqlpp::Row对CCardLevAttrType对象初始化*/
	try
	{
		if (!row) {
			Log("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CCardLevAttrType对象上*/

		m_unId = row["id"];
		m_unLev = row["lev"];
		m_i64Hp = row["hp"];
		m_i64Mp = row["mp"];
		m_i64Atk = row["atk"];
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardLevAttrType::Init  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardLevAttrType::Init  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCardLevAttrType::Init Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardLevAttrType::Init  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
unsigned int CCardLevAttrType::GetId() const {
	/*获取m_unId*/
	return m_unId;
}
unsigned int CCardLevAttrType::GetCardLev() const {
	/*获取m_unCardType*/
	return m_unLev;
}
long long int CCardLevAttrType::GetHp() const {
	/*获取m_i64Hp*/
	return m_i64Hp;
}
long long int CCardLevAttrType::GetMp() const {
	/*获取m_i64Mp*/
	return m_i64Mp;
}
long long int CCardLevAttrType::GetAtk() const {
	/*获取m_i64Atk*/
	return m_i64Atk;
}