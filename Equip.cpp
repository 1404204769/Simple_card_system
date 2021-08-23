#include "Equip.h"
using namespace std;
CEquip::CEquip() {
	/*���캯��*/
	Log("������CEquip���캯��\n");
}
CEquip::~CEquip() {
	/*��������*/
	Log("������CEquip��������\n");
	Update();
}

bool CEquip::CreateNewEquip(const long long int i64UserId, const CEquipType* pEquipType) {
	/*����һ���µ�װ��*/
	if (!pEquipType)
	{
		cout << "װ��������Ч���½�װ��ʧ��" << endl;
		return false;
	}

	if (i64UserId <= 0) {
		cout << "�û�ID���Ϸ�" << endl;
		return false;
	}

	m_i64UserId = i64UserId;
	m_unEquipType = pEquipType->GetId();

	if (!Insert()) {
		cout << "��װ���������ݿ�ʧ��" << endl;
		return false;
	}
	return true;
}
bool CEquip::CreateFromDB(const mysqlpp::Row& row, const CEquipType* pEquipType) {
	/*�����ݿ��м���װ��*/
	try
	{
		if (!row)
		{
			Log("��ʼ��װ������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���ҵ���Ӧ���ݣ�����ӳ�䵽CEquip������*/

		if (!pEquipType)
		{
			cout << "װ��������Ч���½�װ��ʵ��ʧ��" << endl;
			return false;
		}

		m_unEquipType = row["equip_type"];
		if (m_unEquipType != pEquipType->GetId())
		{
			cout << "�������Ͳ�ƥ�䣬�½�����ʧ��" << endl;
			return false;
		}
		m_i64UserId = row["user_id"];
		m_i64EquipId = row["id"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::CreateFromDB()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquip::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
unsigned int CEquip::GetEquipType() const {
	/*��ȡm_unEquipType*/
	return m_unEquipType;
}
long long int CEquip::GetEquipId() const {
	/*��ȡm_i64EquipId*/
	return m_i64EquipId;
}
long long int CEquip::GetUserId() const {
	/*��ȡm_i64UserId*/
	return m_i64UserId;
}
bool CEquip::Insert() {
	/*��Card���ݲ������ݿ�*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CEquip::Insert()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "insert into d_skin values(0,%0q:user_id,%1q:equip_type)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["equip_type"] = m_unEquipType;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CEquip::Insert()  �����ݿ�����û���װ������ʧ��\n");
			return false;
		}
		Log("CEquip::Insert()  �����ݿ�����û���װ�����ݳɹ�\n");

		m_i64EquipId = pQuery->insert_id();
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::Insert()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::Insert()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CEquip::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CEquip::Delete() {
	/*��Card���ݴ����ݿ�ɾ��*/
	try
	{
		Log("CEquip::Delete()\n");
		if (m_i64EquipId == 0) {
			Log("CEquip::Delete()  װ��IdʧЧ\n");
			return false;
		}

		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CEquip::Delete()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "delete from d_equip where id = %0q:EquipId;";
		pQuery->parse();
		pQuery->template_defaults["EquipId"] = m_i64EquipId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CEquip::Delete()  �����ݿ�ɾ���û�װ������ʧ��\n");
			return false;
		}
		Log("CEquip::Delete()  �����ݿ�ɾ���û�װ�����ݳɹ�\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CEquip::Delete()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CEquip::Delete()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CEquip::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkin::Delete()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
//bool CEquip::Update() {
//	/*�����µ����ݸ��µ����ݿ�*/
//	try
//	{
//		Log("CEquip::Update()\n");
//		mysqlpp::Query* pQuery = g_DB.GetQuery();
//		if (!*pQuery) {
//			Log("CEquip::Update()  Query���󲻴��ڣ��޷���������\n");
//			return false;
//		}
//
//		*pQuery << "update `d_equip` set user_id=%0q:UserId,equip_type=%1q:EquipType where id=%2q:id;";
//		pQuery->parse();
//		pQuery->template_defaults["UserId"] = m_i64UserId;
//		pQuery->template_defaults["EquipType"] = m_unEquipType;
//		pQuery->template_defaults["id"] = m_i64EquipId;
//
//		Log("Query:" + pQuery->str() + "\n");
//		if (!g_DB.Update(*pQuery)) {
//			Log("CEquip::Update()�����ݿ����װ��ʧ��\n");
//			return false;
//		}
//		Log("CEquip::Update()�����ݿ����װ���ɹ�\n");
//	}
//	catch (const mysqlpp::BadQuery& er) {
//		Log("CEquip::Update()  Query error: " + string(er.what()) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::BadConversion& er) {
//		Log("CEquip::Update()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::BadIndex& er) {
//		Log("CEquip::Update()  Error: " + string(er.what()) + "\n");
//		return false;
//	}
//	catch (const mysqlpp::Exception& er) {
//		Log("CEquip::Update()  Error: " + string(er.what()) + "\n");
//		return false;
//	}
//	return true;
//}