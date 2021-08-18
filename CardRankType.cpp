#include "CardRankType.h"
using namespace std;
CCardRankType::CCardRankType() {
	/*���캯��*/
	Log("������CCardRankType���캯��\n");
}
CCardRankType::~CCardRankType() {
	/*��������*/
	Log("������CCardRankType��������\n");
}
bool CCardRankType::Init(const mysqlpp::Row& row) {
	/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	/*���ݲ�ѯ��ȡ��mysqlpp::Row��CCardLevAttrType�����ʼ��*/
	try
	{
		if (!row) {
			Log("��ʼ���׼�����ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���Ҵ��Ӧ���ݣ�����ӳ�䵽CCardRankType������*/

		m_unId = row["id"];
		m_unCostCardNum = row["cost_card_num"];
		m_i64Hp = row["hp"];
		m_i64Mp = row["mp"];
		m_i64Atk = row["atk"];
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCardRankType::Init  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCardRankType::Init  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCardRankType::Init Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCardRankType::Init  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
unsigned int CCardRankType::GetId() const {
	/*��ȡm_unId*/
	return m_unId;
}
unsigned int CCardRankType::GetCostCardNum() const {
	/*��ȡm_unCardType*/
	return m_unCostCardNum;
}
long long int CCardRankType::GetHp() const {
	/*��ȡm_i64Hp*/
	return m_i64Hp;
}
long long int CCardRankType::GetMp() const {
	/*��ȡm_i64Mp*/
	return m_i64Mp;
}
long long int CCardRankType::GetAtk() const {
	/*��ȡm_i64Atk*/
	return m_i64Atk;
}