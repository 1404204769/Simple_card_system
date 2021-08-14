#include "SkinType.h"
using namespace std;
CSkinType::CSkinType() {
	/*构造函数*/
	Log("调用了CSkinType构造函数\n");
}
CSkinType::~CSkinType() {
	/*析构函数*/
	Log("调用了CSkinType析构函数\n");
}
bool CSkinType::Init(const mysqlpp::Row& row) {
	/*根据获取的数据库数据初始化静态卡牌数据*/
	/*根据查询获取的mysqlpp::Row对User对象初始化*/
	try
	{
		if (!row) {
			Log("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/

		m_unId = row["id"];
		m_strName = row["name"];
		m_i64Hp = row["hp"];
		m_i64Mp = row["mp"];
		m_i64Atk = row["atk"];
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkinType::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkinType::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkinType::Init() Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkinType::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
const std::string& CSkinType::GetName() const {
	/*获取m_strName*/
	return m_strName;
}
unsigned int CSkinType::GetId() const {
	/*获取m_unId*/
	return m_unId;
}
long long int CSkinType::GetHp() const {
	/*获取m_i64Hp*/
	return m_i64Hp;
}
long long int CSkinType::GetMp() const {
	/*获取m_i64Mp*/
	return m_i64Mp;
}
long long int CSkinType::GetAtk() const {
	/*获取m_i64Atk*/
	return m_i64Atk;
}