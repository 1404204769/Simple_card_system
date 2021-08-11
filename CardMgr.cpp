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
			delete pQuery;
			pQuery = nullptr;
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
			delete pQuery;
			pQuery = nullptr;
			return false;
		}
		delete pQuery;
		pQuery = nullptr;
		strIn << "�����ݿ��ѯ�û��������ݳɹ�\n";
		while (row = res.fetch_row()) {
			CCard* pCard = new CCard();
			if (!pCard) {
				strIn << "����ʵ�廯ʧ��\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
				delete pCard;
				pCard = nullptr;
				return false;
			}
			const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(row["card_type"]);
			if (!pCardType) {
				cout << "�û������еĿ������ͷǷ�,������ʾ" << endl;
				pCardType = nullptr;
				delete pCard;
				pCard = nullptr;
				continue;
			}
			if (!pCard->Init(row["id"], row["user_id"], row["card_type"], string(row["name"]), row["exp"], row["lev"], pCardType)) {
				strIn << "�������ͳ�ʼ��ʧ��\n";
				string strInput(strIn.str());
				OutputDebugPrintf(strInput.c_str());//��ӡ�ڿ���̨
				pCardType = nullptr;
				delete pCard;
				pCard = nullptr;
				return false;
			}
			pCard->SetMark(4);
			m_mapByCardName[pCard->GetName()] = pCard;
			pCardType = nullptr;
			pCard = nullptr;
		}
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
CCard* CCardMgr::GetCardByCardName(std::string& _strName) {
	/*���ݿ������ƻ�ȡ��������*/
	MapByCardNameIter iterByName = m_mapByCardName.find(_strName);
	if (iterByName == m_mapByCardName.end()) {
		cout << "Do not Find CardName:" << _strName << endl;
		return nullptr;
	}
	CCard* pCard= iterByName->second;
	if (!pCard) {
		pCard = nullptr;
		return nullptr;
	} 
	if (pCard->GetMark() == 2) {
		cout << "�޷���ȡ�˿��Ƶ���Ϣ�����������ֳ�����߿�������ɾ����" << endl;
		delete iterByName->second;
		iterByName->second = nullptr;
		m_mapByCardName.erase(iterByName);
		pCard = nullptr;
		return nullptr;
	}
	return pCard;
}
void CCardMgr::PrintAllCard() {
	/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			continue;
		}
		if (pCard->GetMark() == 2) {
			//˵����ʱ�Ѿ���ɾ��
			pCard = nullptr;
			continue;
		}
		cout << "CardID:" << pCard->GetCardId() << "\tUserId:" << pCard->GetUserId() << "\tCardType:" << pCard->GetCardType() << "\tName:" << pCard->GetName() << "\tExp:" << pCard->GetExp() << "\tLev:" << pCard->GetLev() <<"\tMark:"<<pCard->GetMark() << endl;
		const CCardType* pCardType = g_CardTypeMgr.GetCardTypeByType(pCard->GetCardType());
		if (!pCardType) {
			cout << "�޷���ȡ��Ӧ����" << endl;
			pCardType = nullptr;
			pCard = nullptr;
			continue;
		}
		cout << "(ID:" << pCardType->GetId() << "\tCardTypeId:" << pCardType->GetCardType() << "\tCardName:" << pCardType->GetName() << "\tHp:" << pCardType->GetHp() << "\tMp:" << pCardType->GetMp() << "\tAtk:" << pCardType->GetAtk() <<")" << endl;
		pCardType = nullptr;
		pCard = nullptr;
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
		pCard = nullptr;
		cout << "Cardʵ�廯ʧ��" << endl;
		return false;
	}
	string strName;
	cout << "�����뿨�Ƶ��ǳ�(ע�⣺���������п�������)" << endl;
	getline(cin, strName);
	while (GetCardByCardName(strName)) {
		cout << "��������()�Ѵ��ڣ����������뿨�Ƶ��ǳ�(ע�⣺���������п������������ܺ��пո�)" << endl;
		getline(cin, strName);
	}
	if (!pCard->Init(0, _i64UserId, _pCardType->GetCardType(), strName, 0, 1, _pCardType)) {
		cout << "�������ݳ�ʼ��ʧ�ܣ����ӿ���ʧ��" << endl;
		delete pCard;
		pCard = nullptr;
		return false;
	}
	if (!pCard->SetMark(1)) {/*��ǵ�ǰ������������*/
		delete pCard;
		pCard = nullptr;
		return false;
	}
	m_mapByCardName[pCard->GetName()] = pCard;
	pCard = nullptr;
	return true;
}
bool CCardMgr::DelCardByName(std::string& _strName) {
	/*������ҿ���������ɾ������*/
	CCard* pCard = GetCardByCardName(_strName);
	if (!pCard) {
		cout << "ɾ��ʧ�ܣ������û�����Ž�����"<<_strName<<"���Ŀ���" << endl;
		return false;
	}
	if (!pCard->SetMark(2))
		return false;
	return true;
}
bool CCardMgr::DelAllCard() {
	/*ɾ�����еĿ���*/
	bool bRet = true;
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		CCard* pCard = iterByName->second;
		iterByName++;
		if (!pCard) {
			bRet = false;
			continue;
		}
		if (!pCard->SetMark(2)) {
			return false;
		}
	}
	return bRet;
}
void CCardMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	MapByCardNameIter iterByName = m_mapByCardName.begin();
	while (iterByName != m_mapByCardName.end()) {
		delete iterByName->second;//���ͷ��ڴ�
		iterByName->second = nullptr;//�ÿ�
		iterByName++;
	}
	m_mapByCardName.clear();
}