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
		CCardMgr& CardMgr = m_pUser->GetCardMgr();
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

		while (mysqlpp::Row row = res.fetch_row()) {
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

			long long int i64CardId = 0;
			if (pSkin->IsWear()&&CardMgr.Get(pSkin->GetCardId())) {
				i64CardId = pSkin->GetCardId();
			}

			const long long int i64SkinId = pSkin->GetSkinId();
			m_mapBySkinId.insert({ i64SkinId,pSkin.release() });
			if (i64CardId != 0) {
				m_mapByCardId.insert({ i64CardId ,m_mapBySkinId[i64SkinId] });
			}
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
	assert(iterById->second);

	return iterById->second;
}
CSkin* CSkinMgr::GetWearing(const long long int i64CardId) {
	/*����Ƥ��ID��ȡƤ������*/
	CardSkinMapIter iterById = m_mapByCardId.find(i64CardId);
	if (iterById == m_mapByCardId.end()) {
		return nullptr;
	}
	assert(iterById->second);

	return iterById->second;
}
bool CSkinMgr::AddNew( const CSkinType* pSkinType) {
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
	
	const long long int i64UserId = m_pUser->GetId();
	if (!pSkin->CreateNewSkin(i64UserId,pSkinType)) {
		cout << "Ƥ�����ݳ�ʼ��ʧ�ܣ�����Ƥ��ʧ��" << endl;
		return false;
	}

	const long long int i64SkinId = pSkin->GetSkinId();
	m_mapBySkinId.insert({ i64SkinId ,pSkin.release() });
	return true;
}
bool CSkinMgr::Del(long long int i64SkinId) {
	/*����Ƥ��Idɾ��Ƥ��*/
	if (i64SkinId <= 0) {
		cout << "Ƥ��ID���Ϸ�" << endl;
		return false;
	}
	CSkin* pSkin = nullptr;
	if(m_mapBySkinId.count(i64SkinId))
		pSkin = m_mapBySkinId[i64SkinId];

	if (!pSkin) {
		cout << "ɾ��ʧ�ܣ������û������IDΪ��" << i64SkinId << " ��Ƥ��" << endl;
		return false;
	}
	const long long int i64CardId = pSkin->GetCardId();
	if (i64CardId != 0) {
		m_mapByCardId.erase(i64CardId);
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
void CSkinMgr::DeBug_DelAll() {
	/*ɾ�����е�Ƥ��*/
	for(auto &iter:m_mapBySkinId) {
		CSkin* pSkin = iter.second;
		if (!pSkin) {
			continue;
		}
		if (!pSkin->Delete()) {
			cout << "����ɾ��ʧ��" << endl;
		}
		delete pSkin;
	}
	m_mapByCardId.clear();
	m_mapBySkinId.clear();
	return ;
}
void CSkinMgr::PrintAll() {
	/*��ӡ��ʾӵ�е�����Ƥ���Ƶ�����*/
	for (auto& iter : m_mapBySkinId) {
		CSkin* pSkin = iter.second;
		if (!pSkin) {
			continue;
		}
		const unsigned int unSkinType = pSkin->GetSkinType();
		const CSkinType* SkinType = g_SkinTypeMgr.Get(unSkinType);
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
	if (!pSkin->Wear(*pCard)) {
		return false;
	}
	m_mapByCardId.insert({ i64CardId,pSkin });
	return true;
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
	if (!pSkin->Drop(*pCard)) {
		return false;
	}
	m_mapByCardId.erase(i64CardId);
	return true;
}
void CSkinMgr::Free() {
	/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	for (auto& iter:m_mapBySkinId) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapBySkinId.clear();
	m_mapByCardId.clear();
}
