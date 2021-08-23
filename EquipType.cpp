#include "EquipType.h"
using namespace std;
CEquipType::CEquipType() {
	/*构造函数*/
	Log("调用了CEquipType构造函数\n");
}
CEquipType::~CEquipType() {
	/*析构函数*/
	Log("调用了CEquipType析构函数\n");
}

bool CEquipType::Init(const mysqlpp::Row& row) {
	/*根据获取的数据库数据初始化静态装备数据*/
	/*根据查询获取的mysqlpp::Row对User对象初始化*/
	try
	{
		if (!row) {
			Log("初始化用户失败\n");//打印在控制台
			return false;
		}/*否则说明找打对应数据，将其映射到CUser对象上*/

		m_unId = row["id"];
		unsigned int unPos = row["pos"];
		switch (unPos)
		{
			case 0: {
				m_unPos = Prohibit;
			}break;
			case 1: {
				m_unPos = Hat;
			}break;
			case 2: {
				m_unPos = Armour;
			}break;
			case 3: {
				m_unPos = Shoes;
			}break;
			default:
				cout << "数据库读取的装备的类型发现错误" << endl;
			break;
		}
		m_strName = row["name"];
		m_i64Hp = row["hp"];
		m_i64Mp = row["mp"];
		m_i64Atk = row["atk"];
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquipType::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquipType::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquipType::Init() Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquipType::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

const std::string& CEquipType::GetName() const {
	/*获取m_strName*/
	return m_strName;
}
unsigned int CEquipType::GetId() const {
	/*获取m_unId*/
	return m_unId;
}
unsigned int CEquipType::GetPos() const {
	/*获取m_unPos  0表示不可穿戴，1表示穿戴在头部，2表示穿戴在胸口，3表示穿戴在脚上*/
	switch (m_unPos)
	{
	case CEquipType::Prohibit:return 0;
		break;
	case CEquipType::Hat:return 1;
		break;
	case CEquipType::Armour:return 2;
		break;
	case CEquipType::Shoes:return 3;
		break;
	default: {
		cout << "装备类型错误" << endl;
		return -1;
	}break;
	}
	return m_unPos;
}
long long int CEquipType::GetHp() const {
	/*获取m_i64Hp*/
	return m_i64Hp;
}
long long int CEquipType::GetMp() const {
	/*获取m_i64Mp*/
	return m_i64Mp;
}
long long int CEquipType::GetAtk() const {
	/*获取m_i64Atk*/
	return m_i64Atk;
}