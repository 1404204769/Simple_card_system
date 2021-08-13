#include "CardLevAttrType.h"
using namespace std;
CCardLevAttrType::CCardLevAttrType() {
	/*���캯��*/
	Log("������CCardLevAttrType���캯��\n");
}
CCardLevAttrType::~CCardLevAttrType() {
	/*��������*/
	Log("������CCardLevAttrType��������\n");
}
bool CCardLevAttrType::Init(const mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��CCardLevAttrType�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CCardLevAttrType������*/

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
	/*��ȡm_unId*/
	return m_unId;
}
unsigned int CCardLevAttrType::GetCardLev() const {
	/*��ȡm_unCardType*/
	return m_unLev;
}
long long int CCardLevAttrType::GetHp() const {
	/*��ȡm_i64Hp*/
	return m_i64Hp;
}
long long int CCardLevAttrType::GetMp() const {
	/*��ȡm_i64Mp*/
	return m_i64Mp;
}
long long int CCardLevAttrType::GetAtk() const {
	/*��ȡm_i64Atk*/
	return m_i64Atk;
}