#include "Card.h"
#include"Equip.h"
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
		Log("����������Ч���½�����ʧ��\n");
		return false;
	}

	if (i64UserId<=0) {
		Log("�û�ID���Ϸ�\n");
		return false;
	}
	if (!SetLevel(1)) {/*��ʼ�ȼ�Ϊ0*/
		Log("���Ƶȼ���Ч\n");
		return false;
	}
	if (!SetRank(0)) {
		/*��ʼ�׼�Ϊ0*/
		Log("���ƽ׼���Ч\n");
		return false;
	}
	m_i64UserId = i64UserId;
	m_unCardType = pCardType->GetCardType();
	m_strName = pCardType->GetName();
	m_pCardType = pCardType;
	for (int i = 1; i <= EquipPosMax; i++) {
		m_mapEquipPos[i] = 0;
	}
	if (!Insert()) {
		Log("�¿��Ʋ������ݿ�ʧ��\n");
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
			Log("����������Ч���½�����ʧ��\n");
			return false;
		}

		m_unCardType = row["card_type"];
		if (m_unCardType != pCardType->GetCardType())
		{
			Log("�������Ͳ�ƥ�䣬�½�����ʧ��\n");
			return false;
		}

		if (!SetLevel(row["lev"])) {
			Log("���Ƶȼ���Ч\n");
			return false;
		}
		if (!SetRank(row["rank_lev"])) {
			Log("���ƽ׼���Ч\n");
			return false;
		}
		m_i64CardId = row["id"];
		m_i64UserId = row["user_id"];
		m_strName = row["name"];
		m_i64Exp = row["exp"];
		m_pCardType = pCardType;
		string strPos = "";
		for (int i = 1; i <= EquipPosMax; i++) {
			GetFieldNamePos(strPos,i);
			m_mapEquipPos[i] = row[strPos.c_str()];
		}
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

void CCard::GetFieldNamePos(string & strFieldName_Out,const unsigned int unPos) {
	/*��ȡ��Ӧ���ݿ��е�����λ�ֶ���*/
	string strEquip = "equip_pos_";
	strFieldName_Out = strEquip +to_string(unPos);
	return ;
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

long long int CCard::GetEquipPos(const unsigned int unPos) {
	/*��ȡָ��λ���ϵ�װ��*/
	EquipIter iter = m_mapEquipPos.find(unPos);
	if (iter==m_mapEquipPos.end()) {
		Log("��װ��û��װ��λ��"+to_string(unPos)+"\n");
		return 0;
	}
	return iter->second;
}

CCard::EquipIter CCard::GetEquipIterBegin() {
	/*���װ�����������*/
	return m_mapEquipPos.cbegin();
}
CCard::EquipIter CCard::GetEquipIterEnd() {
	/*���װ���������յ�*/
	return m_mapEquipPos.cend();
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


bool CCard::Wear(const long long int i64EquipId, const CEquipType* pEquipType) {
	/*����װ��*/
	if (!pEquipType) {
		Log("�����װ��λ�����ǿ�ָ��\n");
		return false;
	}
	const unsigned int unPos = pEquipType->GetPos();
	EquipIter iter = m_mapEquipPos.find(unPos);
	if (iter==m_mapEquipPos.end()) {
		Log("��װ��û��װ��λ��"+to_string(unPos)+"\n");
		return false;
	}
	if (iter->second!=0) {
		Log("��װ��λ����װ��(ID:"+to_string(iter->second)+")\n");
		return false;
	}
	return SetEquip(i64EquipId, unPos);
}
bool CCard::Drop(const unsigned int unPos) {
	/*����װ��*/
	EquipIter iter = m_mapEquipPos.find(unPos);
	if (iter==m_mapEquipPos.end()) {
		Log("�˿���װ��λ���Ϸ�\n");
		return false;
	}
	if (iter->second == 0) {
		Log("��װ��λδ��װ��\n");
		return false;
	}
	return SetEquip(0, unPos);
}



bool CCard::SetLevel(unsigned int unLev) {
	if (unLev <= 0)
		return false;

	const CCardLevAttrType* pCardLevAttrType = g_CardLevAttrTypeMgr.Get(unLev);
	if (!pCardLevAttrType) {
		Log("���Ƶȼ���������\n");
		return false;
	}

	m_pCardLevAttrType = pCardLevAttrType;
	m_unLev = unLev;
	return true;
}
bool CCard::SetRank(unsigned int unRank) {
	/*����Rank�ȼ�*/
	if (unRank < 0)
		return false;

	const CCardRankType* pCardRankType = g_CardRankTypeMgr.Get(unRank);
	if (!pCardRankType) {
		Log("���ƽ׼���������\n");
		return false;
	}

	m_pCardRankType = pCardRankType;
	m_unRankLev = unRank;
	return true;
}

bool CCard::SetEquip(const long long int i64EquipId,const unsigned int unPos) {
	/*����װ��λ*/
	m_mapEquipPos[unPos]= i64EquipId;
	return true;
}
/*
* ���������ݿ����ؽӿ�
*/


bool CCard::Insert() {
	/*��Card���ݲ������ݿ�*/
	try
	{
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			Log("CCard::Insert()  Queryʵ��ָ�����\n");
			return false;
		}

		*pQuery << "insert into d_card(user_id,card_type,name) values(%0q:user_id, %1q:card_type,%2q:name)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = m_i64UserId;
		pQuery->template_defaults["card_type"] = m_unCardType;
		pQuery->template_defaults["name"] = m_strName.c_str();

		Log("Query:" + pQuery->str() + "\n");

		if (!g_DB.Insert(*pQuery)) {
			Log("CCard::Insert()  �����ݿ�����û��¿�������ʧ��\n");
			return false;
		}
		Log("CCard::Insert()  �����ݿ�����û��¿������ݳɹ�\n");
		m_i64CardId = pQuery->insert_id();

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

		*pQuery << "update `d_card` set user_id=%0q:UserId,`name`=%1q:Name,exp=%2q:Exp,lev=%3q:Lev,rank_lev=%4q:rank_lev,equip_pos_1=%5q:equip_pos_1,equip_pos_2=%6q:equip_pos_2,equip_pos_3=%7q:equip_pos_3 where id = %8q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_i64UserId;
		pQuery->template_defaults["Name"] = m_strName.c_str();
		pQuery->template_defaults["Exp"] = m_i64Exp;
		pQuery->template_defaults["Lev"] = m_unLev;
		pQuery->template_defaults["rank_lev"] = m_unRankLev;
		string StrPos = "";
		for (int i = 1; i <= EquipPosMax; i++) {
			GetFieldNamePos(StrPos,i);
			pQuery->template_defaults[StrPos.c_str()] = m_mapEquipPos[i];
		}
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