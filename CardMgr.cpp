#include "CardMgr.h"
using namespace std;
CCardMgr::CCardMgr() {
	/*���캯��*/
	stringstream strIn;
	strIn << "������CCardMgr���캯��\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
}
CCardMgr::~CCardMgr() {
	/*��������*/
	stringstream strIn;
	strIn << "������CCardMgr��������\n";
	string strInput(strIn.str());
	OutputDebugPrintf(strInput.c_str());
	Free();
}
bool CCardMgr::Init(long long int _i64UserId) {
	/*�����ڲ��������ݿ��ȡ����*/
	try
	{
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		stringstream strIn;
		strIn << "CCardMgr::Init()\n";
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
			return false;
		}
		*pQuery << "select * from d_card where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = _i64UserId;
		strIn << "Query:" << pQuery->str() << "\n";
		if (!g_DB.Search(res, *pQuery)) {
			strIn << "�����ݿ��ѯ�û���������ʧ��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
			return false;
		}
		strIn << "�����ݿ��ѯ�û��������ݳɹ�\n";
		while (row = res.fetch_row()) {
			CCard* pCard = new CCard();
			if (!pCard) {
				strIn << "����ʵ�廯ʧ��\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
				delete pCard;
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(row["card_type"]);
			if (!pCardType) {
				cout << "�û������еĿ������ͷǷ�,������ʾ" << endl;
				delete pCard;
				continue;
			}
			if (!pCard->Init(row["id"], row["user_id"], row["card_type"], string(row["name"]), row["exp"], row["lev"], pCardType)) {
				strIn << "�������ͳ�ʼ��ʧ��\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
				delete pCard;
				return false;
			}
			m_mapByCardId[pCard->GetCardId()] = pCard;
		}
		m_vecDel.clear();
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCardMgr::Init()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());;
		return false;
	}
	return true;
}
CCard* CCardMgr::GetCardByCardId(long long int _i64CardId) {
	/*���ݿ������ƻ�ȡ��������*/
	MapByCardIdIter iterByName = m_mapByCardId.find(_i64CardId);
	if (iterByName == m_mapByCardId.end()) {
		cout << "Do not Find CardId:" << _i64CardId << endl;
		return nullptr;
	}
	return iterByName->second;
}
void CCardMgr::PrintAllCard() {
	/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	MapByCardIdIter iterById = m_mapByCardId.begin();
	while (iterById != m_mapByCardId.end()) {
		CCard* pCard = iterById->second;
		iterById++;
		if (!pCard) {
			continue;
		}
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tCardType:" << pCard->GetCardType() << "\tName:" << pCard->GetName() << "\tExp:" << pCard->GetExp() << "\tLev:" << pCard->GetLev() <<"\tMark:"<<pCard->GetMark() << endl;
		const CCardType* pCardType = pCard->GetCardTypeData();
		if (!pCardType) {
			cout << "�޷���ȡ��Ӧ����" << endl;
			continue;
		}
		cout << "\t(ID:" << pCardType->GetId() << "\tCardTypeId:" << pCardType->GetCardType() << "\tCardName:" << pCardType->GetName() << "\tHp:" << pCardType->GetHp() << "\tMp:" << pCardType->GetMp() << "\tAtk:" << pCardType->GetAtk() <<")" << endl;
	}
}
bool CCardMgr::AddCard(long long int _i64UserId, const CCardType* _pCardType) {
	/*����һ��ָ�����͵Ŀ���*/
	if (!_pCardType) {
		cout << "����������Ч" << endl;
		return false;
	}
	CCard* pCard = new CCard();
	if (!pCard) {
		delete pCard;
		cout << "Cardʵ�廯ʧ��" << endl;
		return false;
	}
	long long int i64CardId = 0;
	if (!Insert(i64CardId, _i64UserId, _pCardType->GetCardType(), "", 0, 1)) {
		cout << "�û��������Ʋ������ݿ�ʧ��" << endl;
		return false;
	}
	if (!pCard->Init(i64CardId, _i64UserId, _pCardType->GetCardType(), "", 0, 1,_pCardType)) {
		cout << "�������ݳ�ʼ��ʧ�ܣ����ӿ���ʧ��" << endl;
		delete pCard;
		return false;
	}
	m_mapByCardId[pCard->GetCardId()] = pCard;
	return true;
}
bool CCardMgr::DelCardById(long long int _i64CardId) {
	/*������ҿ���������ɾ������*/
	CCard* pCard = m_mapByCardId[_i64CardId];
	if (!pCard) {
		cout << "ɾ��ʧ�ܣ������û�����Ž�����"<<_i64CardId<<"���Ŀ���" << endl;
		return false;
	}
	m_mapByCardId.erase(_i64CardId);
	m_vecDel.push_back(pCard);
	return true;
}
bool CCardMgr::DelAllCard() {
	/*ɾ�����еĿ���*/
	bool bRet = true;
	MapByCardIdIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}
		m_vecDel.push_back(pCard);
	}
	m_mapByCardId.clear();
	return bRet;
}
void CCardMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	MapByCardIdIter iterByName = m_mapByCardId.begin();
	while (iterByName != m_mapByCardId.end()) {
		delete iterByName->second;//���ͷ��ڴ�
		iterByName->second = nullptr;//�ÿ�
		iterByName++;
	}
	m_mapByCardId.clear();
	vector<CCard*>::iterator VecIter = m_vecDel.begin();
	while (VecIter != m_vecDel.end()) {
		CCard* pCard = *VecIter++;
		if(!pCard)
			continue;
		Delete(*pCard);
	}
	m_vecDel.clear();
}


/*���ݿ���ؽӿ�*/
bool CCardMgr::Insert(long long int& _i64CardId, const long long int _i64UserId, const unsigned int _unCardType, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev){
	/*��Card���ݲ������ݿ�*/
	/*�����ݿ��в����µ�User����*/
	try
	{
		stringstream strIn;
		strIn << "CCard::Insert()\n";
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		*pQuery << "insert into d_card values(0,%0q:user_id, %1q:card_type,%2q:name,%3q:exp,%4q:lev)";
		pQuery->parse();
		pQuery->template_defaults["user_id"] = _i64UserId;
		pQuery->template_defaults["card_type"] = _unCardType;
		pQuery->template_defaults["name"] = _strName.c_str();
		pQuery->template_defaults["exp"] = _i64Exp;
		pQuery->template_defaults["lev"] = _unLev;
		bool bRet = g_DB.Insert(*pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		if (!bRet) {
			strIn << "�����ݿ�����û��¿�������ʧ��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn << "�����ݿ�����û��¿������ݳɹ�\n";
		pQuery->reset();
		*pQuery << "select @@IDENTITY as ID";
		pQuery->parse();
		mysqlpp::UseQueryResult res;
		if (!g_DB.Search(res, *pQuery)) {
			cout << "��ȡϵͳ�����CardIdʧ��" << endl;
			return false;
		}
		mysqlpp::Row row=res.fetch_row();
		if (!row) {
			cout << "û�л�ȡ��ϵͳ�����CardId" << endl;
			return false;
		}
		_i64CardId = row["ID"];
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCard::Insert()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}
bool CCardMgr::Delete(CCard& Card) {
	/*��Card���ݴ����ݿ�ɾ��*/
	try
	{
		stringstream strIn;
		strIn << "CCard::Delete()\n";
		mysqlpp::Query* pQuery = g_DB.GetQuery();
		if (!*pQuery) {
			strIn << "Queryʵ��ָ�����\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		*pQuery << "delete from d_card where id = %0q:CardId;";
		pQuery->parse();
		pQuery->template_defaults["CardId"] = Card.GetCardId();
		bool bRet = g_DB.Delete(*pQuery);
		strIn << "Query:" << pQuery->str() << "\n";
		if (!bRet) {
			strIn << "�����ݿ�ɾ���û���������ʧ��\n";
			string strInput(strIn.str());
			OutputDebugPrintf(strInput.c_str());
			return false;
		}
		strIn << "�����ݿ�ɾ���û��������ݳɹ�\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
	}
	catch (const mysqlpp::BadQuery& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nQuery error: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nConversion error: " << er.what() << "\ntretrieved data size: " << er.retrieved << ", actual size: " << er.actual_size << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		stringstream strIn;
		strIn << "CCard::Delete()\nError: " << er.what() << "\n";
		string strInput(strIn.str());
		OutputDebugPrintf(strInput.c_str());
		return false;
	}
	return true;
}