#include "CardType.h"
using namespace std;
CCardType::CCardType() {
	/*���캯��*/
	Log("������CCardType���캯��\n");
}
CCardType::~CCardType() {
	/*��������*/
	Log("������CCardType��������\n");
}
bool CCardType::Init(const mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CUser������*/

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
	/*��ȡm_strName*/
	return m_strName;
}
unsigned int CCardType::GetId() const {
	/*��ȡm_unId*/
	return m_unId;
}
unsigned int CCardType::GetCardType() const {
	/*��ȡm_unCardType*/
	return m_unCardType;
}
long long int CCardType::GetHp() const {
	/*��ȡm_i64Hp*/
	return m_i64Hp;
}
long long int CCardType::GetMp() const {
	/*��ȡm_i64Mp*/
	return m_i64Mp;
}
long long int CCardType::GetAtk() const {
	/*��ȡm_i64Atk*/
	return m_i64Atk;
}