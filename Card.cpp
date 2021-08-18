#include "Card.h"
using namespace std;
CCard::CCard() {
	/*���캯��*/
	Log("������CCard���캯��\n");
}
CCard::~CCard(){
	/*��������*/
	Log("������CCard��������\n");

	if (!Update())
		Log("�û��������ݱ���ʧ��\n");
	else
		Log("�û��������ݱ���ɹ�\n");
}

bool CCard::CreateNewCard(const long long int i64UserId, const CCardType* pCardType) {
	/*����һ���µĿ���*/
	if (!pCardType)
	{
		cout << "����������Ч���½�����ʧ��" << endl;
		return false;
	}

	if (i64UserId<=0) {
		cout << "�û�ID���Ϸ�" << endl;
		return false;
	}

	if (!SetLevel(1)) {/*��ʼ�ȼ�Ϊ0*/
		cout << "���Ƶȼ���Ч" << endl;
		return false;
	}
	if (!SetRank(0)) {
		/*��ʼ�׼�Ϊ0*/
		cout << "���ƽ׼���Ч" << endl;
		return false;
	}
	m_i64UserId = i64UserId;
	m_unCardType = pCardType->GetCardType();
	m_strName = pCardType->GetName();
	m_i64Exp = 0;
	m_pCardType = pCardType;

	if (!Insert(m_i64CardId)) {
		cout << "�¿��Ʋ������ݿ�ʧ��" << endl;
		return false;
	}
	return true;
}
bool CCard::CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType) {
	/*�����ݿ��м��ؿ���*/
	try
	{
		if (!row) 
		{
			Log("��ʼ������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}/*����˵���ҵ���Ӧ���ݣ�����ӳ�䵽Card������*/

		if (!pCardType)
		{
			cout << "����������Ч���½�����ʧ��" << endl;
			return false;
		}

		m_unCardType = row["card_type"];
		if (m_unCardType != pCardType->GetCardType())
		{
			cout << "�������Ͳ�ƥ�䣬�½�����ʧ��" << endl;
			return false;
		}

		if (!SetLevel(row["lev"])) {
			cout << "���Ƶȼ���Ч" << endl;
			return false;
		}
		if (!SetRank(row["rank_lev"])) {
			cout << "���ƽ׼���Ч" << endl;
			return false;
		}
		m_i64CardId = row["id"];
		m_i64UserId = row["user_id"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_pCardType = pCardType;

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::CreateFromDB()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::CreateFromDB()  Conversion error: " +string( er.what()) + ",tretrieved data size: " + to_string( er.retrieved)+ ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::CreateFromDB()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

const std::string& CCard::GetName() const{
	/*��ȡm_strName*/
	return m_strName;
}
unsigned int CCard::GetLev() const {
	/*��ȡm_unLev*/
	return m_unLev;
}
unsigned int CCard::GetCardRankLev() const {
	/*��ȡm_unRankLev*/
	return m_unRankLev;
}
unsigned int CCard::GetCardType() const{
	/*��ȡm_unCardType*/
	return m_unCardType;
}
long long int CCard::GetCardId() const{
	/*��ȡm_i64CardId*/
	return m_i64CardId;
}
long long int CCard::GetUserId() const{
	/*��ȡm_i64UserId*/
	return m_i64UserId;
}
long long int CCard::GetExp() const{
	/*��ȡm_i64Exp*/
	return m_i64Exp;
}
const CCardType& CCard::GetCardTypeData() const{
	/*��ȡ��Ӧ�Ŀ���������ϸ����*/
	assert(m_pCardType);
	return *m_pCardType;
}
const CCardLevAttrType& CCard::GetCardLevAttrTypeData() const {
	/*��ȡ��Ӧ���Ƶȼ��ļӳ�����*/
	assert(m_pCardLevAttrType);

	return *m_pCardLevAttrType;
}
const CCardRankType& CCard::GetCardRankTypeData() const {
	/*��ȡ��Ӧ���ƽ׼��ļӳ�����*/
	assert(m_pCardRankType);

	return *m_pCardRankType;
}
bool CCard::LevelUp(unsigned int unLev) {
	/*�������Ƶȼ��������ӳ�����*/
	return SetLevel(unLev + m_unLev);
}
bool CCard::RankUp() {
	/*�������ƽ׼��������ӳ�����*/
	return SetRank(m_unRankLev+1);
}
bool CCard::SetLevel(unsigned int unLev) {
	if (unLev <= 0)
		return false;

	const CCardLevAttrType* pCardLevAttrType = g_CardLevAttrTypeMgr.Get(unLev);
	if (!pCardLevAttrType) {
		cout << "���Ƶȼ���������" << endl;
		return false;
	}

	m_pCardLevAttrType = pCardLevAttrType;
	m_unLev = unLev;
	return true;
}
bool CCard::SetRank(unsigned int unRank) {
	/*����Rank�ȼ�*/
	if (unRank <= 0)
		return false;

	const CCardRankType* pCardRankType = g_CardRankTypeMgr.Get(unRank);
	if (!pCardRankType) {
		cout << "���ƽ׼���������" << endl;
		return false;
	}

	m_pCardRankType = pCardRankType;
	m_unRankLev = unRank;
	return true;
}

/*
* ���������ݿ����ؽӿ�
*/


bool CCard::Insert(long long int& i64CardId_Out) {
	/*��Card���ݲ������ݿ�*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Insert()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "insert into d_card values(0,%0q:user_id, %1q:card_type,%2q:name,%3q:exp,%4q:lev��%5q:rank_lev)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["card_type"] = m_unCardType;
		pQuery->template_defaults["name"] = m_strName.c_str();
		pQuery->template_defaults["exp"] = m_i64Exp;
		pQuery->template_defaults["lev"] = m_unLev;
		pQuery->template_defaults["rank_lev"] = m_unRankLev;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CCard::Insert()  �����ݿ�����û��¿�������ʧ��\n");
			return false;
		}
		Log("CCard::Insert()  �����ݿ�����û��¿������ݳɹ�\n");

		pQuery->reset();
		*pQuery << "select @@IDENTITY as ID";
		pQuery->parse();

		mysqlpp::UseQueryResult res;
		if (!g_DB.Search(res, *pQuery)) {
			Log("CCard::Insert()  ��ȡϵͳ�����CardIdʧ��\n");
			return false;
		}
		mysqlpp::Row row = res.fetch_row();
		if (!row) {
			Log("CCard::Insert()  û�л�ȡ��ϵͳ�����CardId\n");
			return false;
		}
		i64CardId_Out = row["ID"];

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Insert()  Query error: " + string(er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Insert()  Conversion error: " +string( er.what()) + ",tretrieved data size: " +to_string( er.retrieved) + ", actual size: " +to_string(er.actual_size) +"\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Insert()  Error: " +string( er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Insert()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}
bool CCard::Delete() {
	/*��Card���ݴ����ݿ�ɾ��*/
	try
	{
		Log("CCard::Delete()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Delete()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "delete from d_card where id = %0q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["CardId"] =m_i64CardId;

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Delete(*pQuery)) {
			Log("CCard::Delete()  �����ݿ�ɾ���û���������ʧ��\n");
			return false;
		}
		Log("CCard::Delete()  �����ݿ�ɾ���û��������ݳɹ�\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Delete()  Query error: " +string( er.what())+"\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Delete()  Conversion error: "+ string( er.what())+ ",tretrieved data size: " +to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size)+ "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Delete()  Error: " +string( er.what() )+ "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Delete()  Error: "+ string( er.what())+ "\n");
		return false;
	}
	return true;
}
bool CCard::Update() {
	/*�������ݿ�*/
	try
	{
		Log("CCard::Update()\n");
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Update()  Query���󲻴��ڣ��޷���������\n");
			return false;
		}

		*pQuery << "update `d_card` set user_id=%0q:UserId,`name`=%1q:Name,exp=%2q:Exp,lev=%3q:Lev where id = %4q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_i64UserId;
		pQuery->template_defaults["Name"] = m_strName.c_str();
		pQuery->template_defaults["Exp"] = m_i64Exp;
		pQuery->template_defaults["Lev"] = m_unLev;
		pQuery->template_defaults["CardId"] = m_i64CardId;

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Update(*pQuery)) {
			Log("CCard::Update()  �����ݿ�����û�����ʧ��\n");
			return false;
		}

		Log("CCard::Update()  �����ݿ�����û����Ƴɹ�\n");
	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CCard::Update()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CCard::Update()  Conversion error: " +string( er.what() )+",tretrieved data size: " +to_string(er.retrieved)  + ", actual size: " +to_string(er.actual_size)  + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CCard::Update()  Error: " +string( er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CCard::Update()  Error: " +string( er.what()) + "\n");
		return false;
	}
	return true;
}