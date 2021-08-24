#include "EquipType.h"
using namespace std;
CEquipType::CEquipType() {
	/*���캯��*/
	Log("������CEquipType���캯��\n");
}
CEquipType::~CEquipType() {
	/*��������*/
	Log("������CEquipType��������\n");
}

bool CEquipType::Init(const mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬װ������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/

		m_unId = row["id"];
		m_unPos = row["pos"];
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
	/*��ȡm_strName*/
	return m_strName;
}
unsigned int CEquipType::GetId() const {
	/*��ȡm_unId*/
	return m_unId;
}
unsigned int CEquipType::GetPos() const {
	/*��ȡm_unPos  0��ʾ���ɴ�����1��ʾ������ͷ����2��ʾ�������ؿڣ�3��ʾ�����ڽ���*/
	return m_unPos;
}
long long int CEquipType::GetHp() const {
	/*��ȡm_i64Hp*/
	return m_i64Hp;
}
long long int CEquipType::GetMp() const {
	/*��ȡm_i64Mp*/
	return m_i64Mp;
}
long long int CEquipType::GetAtk() const {
	/*��ȡm_i64Atk*/
	return m_i64Atk;
}