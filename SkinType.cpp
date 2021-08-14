#include "SkinType.h"
using namespace std;
CSkinType::CSkinType() {
	/*���캯��*/
	Log("������CSkinType���캯��\n");
}
CSkinType::~CSkinType() {
	/*��������*/
	Log("������CSkinType��������\n");
}
bool CSkinType::Init(const mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/

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
	/*��ȡm_strName*/
	return m_strName;
}
unsigned int CSkinType::GetId() const {
	/*��ȡm_unId*/
	return m_unId;
}
long long int CSkinType::GetHp() const {
	/*��ȡm_i64Hp*/
	return m_i64Hp;
}
long long int CSkinType::GetMp() const {
	/*��ȡm_i64Mp*/
	return m_i64Mp;
}
long long int CSkinType::GetAtk() const {
	/*��ȡm_i64Atk*/
	return m_i64Atk;
}