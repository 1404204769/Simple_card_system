#include "Skin.h"
#include "Card.h"
using namespace std;
CSkin::CSkin() {
	/*���캯��*/
	Log("������CSkin���캯��\n");
}
CSkin::~CSkin() {
	/*��������*/
	Log("������CCard��������\n");
}

bool CSkin::CreateNewSkin(const long long int i64UserId, const CSkinType* pSkinType) {
	/*����һ���µ�Ƥ��*/
	if (!pSkinType)
	{
		cout << "Ƥ��������Ч���½�����ʧ��" << endl;
		return false;
	}

	if (i64UserId <= 0) {
		cout << "�û�ID���Ϸ�" << endl;
		return false;
	}

	m_i64UserId = i64UserId;
	m_unSkinType = pSkinType->GetId();
	m_i64CardId = 0;

	if (!Insert(m_i64SkinId)) {
		cout << "��Ƥ���������ݿ�ʧ��" << endl;
		return false;
	}
	return true;
}
bool CSkin::CreateFromDB(const mysqlpp::Row& row, const CSkinType* pSkinType) {
	/*�����ݿ��м���Ƥ������*/
	try
	{
		if (!row)
		{
			Log("��ʼ��Ƥ������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���ҵ���Ӧ���ݣ�����ӳ�䵽CSkin������*/

		if (!pSkinType)
		{
			cout << "Ƥ��������Ч���½�Ƥ��ʵ��ʧ��" << endl;
			return false;
		}

		m_unSkinType = row["skin_type"];
		if (m_unSkinType != pSkinType->GetId())
		{
			cout << "�������Ͳ�ƥ�䣬�½�����ʧ��" << endl;
			return false;
		}

		m_i64CardId = 0;
		m_i64UserId = row["user_id"];
		m_i64SkinId = row["id"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::CreateFromDB()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

unsigned int CSkin::GetSkinType() const {
	/*��ȡm_unSkindType*/
	return m_unSkinType;
}
long long int CSkin::GetCardId() const {
	/*��ȡm_i64CardId*/
	return m_i64CardId;
}
long long int CSkin::GetSkinId() const {
	/*��ȡm_i64SkinId*/
	return m_i64SkinId;
}
long long int CSkin::GetUserId() const {
	/*��ȡm_i64UserId*/
	return m_i64UserId;
}
bool CSkin::IsWear()const {
	if (m_i64CardId == 0)/*��ʱ��û�п��ƴ������Ƥ��*/
		return false;
	return true;
}

bool CSkin::Wear(CCard& Card) {
	/*����ĳ��Ƥ��*/
	if (!IsWear()) {//�����û����
		if (Card.GetSkinId()) {//Ŀ�꿨��Ҳ��û���κ�Ƥ��
			cout << "Ƥ��ID" << m_i64SkinId << "����ʧ��" << endl;
			return false;
		}
		Card.SetSkinId(m_i64SkinId);
		m_i64CardId = Card.GetCardId();
		return true;
	}
	return false;
}
bool CSkin::Drop(CCard& Card) {
	/*����ĳ��Ƥ��*/
	if (!IsWear()) {//��û����
		cout << "Ƥ��ID:"<< m_i64SkinId <<"��û������" << endl;
		return false;
	}

	if (m_i64CardId != Card.GetCardId()) {
		cout << "Ƥ��ID:" << m_i64SkinId << " ���ڿ���ID��"<< m_i64CardId <<"  ���࿨�Ʋ��ö������" << endl;
		return false;
	}

	if (Card.GetSkinId()==0) {
		cout << "Ƥ��ID" << m_i64SkinId << "����ʧ��" << endl;
		return false;
	}
	Card.SetSkinId(0);
	m_i64CardId = 0;
	return true;
}

/*
* ���ݿ���ز���
*/

bool CSkin::Delete() {
	/*��Skin���ݴ����ݿ�ɾ��*/
	try
	{
		Log("CSkin::Delete()\n");
		if (m_i64SkinId == 0) {
			Log("CSkin::Delete()  Ƥ��IdʧЧ\n");
			return false;
		}

		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CSkin::Delete()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "delete from d_skin where id = %0q:SkinId;";
		pQuery->parse();
		pQuery->template_defaults["SkinId"] = m_i64SkinId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CSkin::Delete()  �����ݿ�ɾ���û�Ƥ������ʧ��\n");
			return false;
		}
		Log("CSkin::Delete()  �����ݿ�ɾ���û�Ƥ�����ݳɹ�\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::Delete()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::Delete()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CSkin::Insert(long long int& i64SkinId_Out) {
	/*��Skin���ݲ������ݿ�*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CSkin::Insert()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "insert into d_skin values(0,%0q:user_id, %1q:skin_type)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["skin_type"] = m_unSkinType;
		
		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CSkin::Insert()  �����ݿ�����û���Ƥ������ʧ��\n");
			return false;
		}
		Log("CSkin::Insert()  �����ݿ�����û���Ƥ�����ݳɹ�\n");

		i64SkinId_Out = pQuery->insert_id();
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkin::Insert()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkin::Insert()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkin::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
