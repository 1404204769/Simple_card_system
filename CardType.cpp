#include "CardType.h"
using namespace std;
CCardType::CCardType() {
	/*���캯��*/
	stringstream strIn;
	strIn << "������CCardType���캯��\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCardType::~CCardType() {
	/*��������*/
	stringstream strIn;
	strIn << "������CCardType��������\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
bool CCardType::Init(mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��User�����ʼ��*/
	try
	{
		if (!row) {
			OutputDebugPrintf("��ʼ���û�ʧ��\n");//��ӡ�ڿ���̨
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
		stringstream strIn;
		strIn << "CCardType::Init()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCardType::Init()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCardType::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCardType::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
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