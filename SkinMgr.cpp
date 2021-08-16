#include "SkinMgr.h"
#include "User.h"
#include "CardMgr.h"
using namespace std;
CSkinMgr::CSkinMgr() {
	/*���캯��*/
	Log("������CSkinMgr���캯��\n");
}
CSkinMgr::~CSkinMgr() {
	/*��������*/
	Log("������CSkinMgr��������\n");

	Free();
	m_pUser = nullptr;
}

bool CSkinMgr::Init(CUser* pUser) {
	/*�����ڲ��������ݿ��ȡ����*/
	try
	{
		if (!pUser)
		{
			cout << "����Ƥ��������ʱ�����û�������" << endl;
			return false;
		}

		m_pUser = pUser;
		mysqlpp::Row row;
		mysqlpp::UseQueryResult res;
		mysqlpp::Query* pQuery = g_DB.GetQuery();

		Log("SkinMgr::Init()\n");
		if (!*pQuery) {
			Log("Queryʵ��ָ�����\n");//��ӡ�ڿ���̨
			return false;
		}

		*pQuery << "select * from d_skin where user_id = %0q:UserId;";
		pQuery->parse();
		pQuery->template_defaults["UserId"] = m_pUser->GetId();

		Log("Query:" + pQuery->str() + "\n");
		if (!g_DB.Search(res, *pQuery)) {
			Log("SkinMgr::Init()  �����ݿ��ѯ�û�Ƥ������ʧ��\n");//��ӡ�ڿ���̨
			return false;
		}
		Log("CSkinMgr::Init()  �����ݿ��ѯ�û�Ƥ�����ݳɹ�\n");

		while (row = res.fetch_row()) {
			unique_ptr<CSkin> pSkin(new CSkin());
			if (!pSkin) {
				Log("CSkinMgr::Init()  Ƥ��ʵ�廯ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			const CSkinType* pSkinType = g_SkinTypeMgr.Get(row["skin_type"]);
			if (!pSkinType) {
				cout << "�û������е�Ƥ�����ͷǷ�,������ʾ" << endl;
				continue;
			}
			if (!pSkin->CreateFromDB(row, pSkinType)) {
				Log("CSkinMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			m_mapBySkinId[pSkin->GetSkinId()] = pSkin.release();
		}

	}
	catch (const mysqlpp::BadQuery& er) {
		Log("CSkinMgr::Init()  Query error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::BadConversion& er) {
		Log("CSkinMgr::Init()  Conversion error: " + string(er.what()) + ",tretrieved data size: " + to_string(er.retrieved) + ", actual size: " + to_string(er.actual_size) + "\n");
		return false;
	}
	catch (const mysqlpp::BadIndex& er) {
		Log("CSkinMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	catch (const mysqlpp::Exception& er) {
		Log("CSkinMgr::Init()  Error: " + string(er.what()) + "\n");
		return false;
	}
	return true;
}

CSkin* CSkinMgr::Get(const long long int i64SkinId) {
	/*���ݿ������ƻ�ȡ��������*/
	SkinMapIter iterById = m_mapBySkinId.find(i64SkinId);
	if (iterById == m_mapBySkinId.end()) {
		cout << "Do not Find SkinId:" << i64SkinId << endl;
		return nullptr;
	}
	return iterById->second;
}

bool CSkinMgr::Add(const long long int i64UserId, const CSkinType* pSkinType) {
	/*���һ����Ƥ��*/
	if (!m_pUser)
	{
		cout << "��Ƥ�����������û�ʧ����" << endl;
		return false;
	}

	if (!pSkinType) {
		cout << "Ƥ��������Ч" << endl;
		return false;
	}

	unique_ptr<CSkin> pSkin(new CSkin());
	if (!pSkin) {
		cout << "Skinʵ�廯ʧ��" << endl;
		return false;
	}

	if (!pSkin->CreateNewSkin(i64UserId,pSkinType)) {
		cout << "Ƥ�����ݳ�ʼ��ʧ�ܣ�����Ƥ��ʧ��" << endl;
		return false;
	}
	m_mapBySkinId[pSkin->GetSkinId()] = pSkin.release();
	return true;
}
bool CSkinMgr::Del(long long int i64SkinId) {
	/*����Ƥ��Idɾ��Ƥ��*/
	if (i64SkinId <= 0) {
		cout << "Ƥ��ID���Ϸ�" << endl;
		return false;
	}

	CSkin* pSkin = m_mapBySkinId[i64SkinId];
	m_mapBySkinId[i64SkinId] = nullptr;

	if (!pSkin) {
		cout << "ɾ��ʧ�ܣ������û������IDΪ��" << i64SkinId << " ��Ƥ��" << endl;
		return false;
	}

	m_mapBySkinId.erase(i64SkinId);
	if (!pSkin->Delete())
	{
		delete pSkin;
		return false;
	}
	delete pSkin;
	return true;
}
bool CSkinMgr::DelAll() {
	/*ɾ�����е�Ƥ��*/
	bool bRet = true;
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		CSkin* pSkin = iterById->second;
		iterById->second = nullptr;
		iterById++;
		if (!pSkin) {
			bRet = false;
			continue;
		}

		if (!pSkin->Delete()) {
			cout << "����ɾ��ʧ��" << endl;
			bRet = false;
		}
		delete pSkin;
	}
	m_mapBySkinId.clear();
	return bRet;
}
void CSkinMgr::PrintAll() {
	/*��ӡ��ʾӵ�е�����Ƥ���Ƶ�����*/
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		CSkin* pSkin = iterById->second;
		iterById++;
		if (!pSkin) {
			continue;
		}
		const CSkinType* SkinType = g_SkinTypeMgr.Get(pSkin->GetSkinType());
		if (!SkinType) {
			cout << "Ƥ�����Ͳ�����" << endl;
			continue;
		}
		cout << "SkinID:" << pSkin->GetSkinId() << "\tUserId:" << pSkin->GetUserId() << "\tWear:" << pSkin->GetCardId() 
			<< "\tSkinType:" << SkinType->GetId() << "\tSkinName:" << SkinType->GetName() 
			<< "\tSkinAtk:" << SkinType->GetAtk() << "\tSkinHp:" << SkinType->GetHp() << "\tSkinMp:" << SkinType->GetMp() << endl;
	}
}
bool CSkinMgr::Wear(const long long int i64CardId, const long long int i64SkinId) {
	/*ָ��ĳ�ſ��ƴ���ĳ��Ƥ��*/
	if (!m_pUser) {
		cout << "�û���Ƥ��������ʧ����" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "û���ҵ�Ҫ����Ƥ���Ŀ���" << endl;
		return false;
	}
	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		cout << "û���ҵ�Ҫ������Ƥ��" << endl;
		return false;
	}
	if (pSkin->IsWear()) {
		cout << "Ҫ������Ƥ���Ѿ�������("<<pSkin->GetCardId() << ")������" << endl;
		return false;
	}
	return pSkin->Wear(*pCard);
}
bool CSkinMgr::Drop(const long long int i64CardId, const long long int i64SkinId) {
	/*ָ��ĳ�ſ�������ĳ��Ƥ��*/
	if (!m_pUser) {
		cout << "�û���Ƥ��������ʧ����" << endl;
		return false;
	}
	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		cout << "û���ҵ�Ҫ����Ƥ���Ŀ���" << endl;
		return false;
	}
	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		cout << "û���ҵ�Ҫ���µ�Ƥ��" << endl;
		return false;
	}
	if (!pSkin->IsWear()) {
		cout << "Ҫ���µ�Ƥ����������ʹ��" << endl;
		return false;
	}
	return pSkin->Drop(*pCard);
}
void CSkinMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	SkinMapIter iterById = m_mapBySkinId.begin();
	while (iterById != m_mapBySkinId.end()) {
		delete iterById->second;//���ͷ��ڴ�
		iterById->second = nullptr;//�ÿ�
		iterById++;
	}
	m_mapBySkinId.clear();
}
