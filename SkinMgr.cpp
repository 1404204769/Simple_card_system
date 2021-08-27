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
			Log( "����Ƥ��������ʱ�����û�������\n" );
			return false;
		}

		m_pUser = pUser;
		CCardMgr& CardMgr = m_pUser->GetCardMgr();
		if (!CardMgr.IsInit()) {
			Log("���ƹ�������δ��ʼ��");
			return false;
		}
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
				Log( "�û������е�Ƥ�����ͷǷ�,������ʾ\n" );
				continue;
			}
			if (!pSkin->CreateFromDB(row, pSkinType)) {
				Log("CSkinMgr::Init()  �������ͳ�ʼ��ʧ��\n");//��ӡ�ڿ���̨
				return false;
			}
			if (!Add(pSkin.release())) {
				Log( "��Ƥ����ID:" + to_string(pSkin->GetSkinId())+"�������ڴ�ʧ��\n" );
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
		Log("Do not Find SkinId:"+to_string(i64SkinId)+"\n");
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
		Log("��Ƥ�����������û�ʧ����\n");
		return false;
	}

	if (!pSkinType) {
		Log( "Ƥ��������Ч\n");
		return false;
	}
	const unsigned int unSkinType = pSkinType->GetId();
	SkinTypeSetIter iterSkinType = m_setBySkinType.find(unSkinType);
	if (iterSkinType!=m_setBySkinType.end()) {
		Log("����Ƥ����ӵ�У�����ʧ��\n");
		return false;
	}

	unique_ptr<CSkin> pSkin(new CSkin());
	if (!pSkin) {
		Log( "Skinʵ�廯ʧ��\n" );
		return false;
	}
	
	const long long int i64UserId = m_pUser->GetId();
	if (!pSkin->CreateNewSkin(i64UserId,pSkinType)) {
		Log( "Ƥ�����ݳ�ʼ��ʧ�ܣ�����Ƥ��ʧ��\n" );
		return false;
	}
	return Add(pSkin.release());
}
bool CSkinMgr::Add(CSkin* pSkin){
	/*�������Ӳ�����choice=1����������Ƥ������,choice=2:������Ƥ��*/
	if (!m_pUser) {
		Log( "��Ƥ�����������û�ʧ����\n");
		return false;
	}
	if (!pSkin) {
		Log("�����Ƥ��ָ��Ϊ��\n");
		return false;
	}

	const unsigned int unSkinType = pSkin->GetSkinType();
	SkinTypeSetIter iterSkinType = m_setBySkinType.find(unSkinType);
	if (iterSkinType!=m_setBySkinType.end()) {
		Log("����Ƥ����ӵ�У�����ʧ��\n");
		return false;
	}

	const long long int i64SkinId = pSkin->GetSkinId();
	const long long int i64UserId = pSkin->GetUserId();
	const long long int i64CardId = pSkin->GetCardId();

	SkinMapIter iterSkin = m_mapBySkinId.find(i64SkinId);
	if (iterSkin!=m_mapBySkinId.end()) {
		Log( "��Ƥ���Ѵ��ڣ�����ʧ��\n" );
		return false;
	}

	m_setBySkinType.insert(unSkinType);//��¼��������
	m_mapBySkinId.insert({ i64SkinId ,pSkin });//����Ƥ��ʵ��

	if (!pSkin->IsWear() ) {//��Ƥ��û�б��������ٽ��в���
		return true;
	}
	CCardMgr CardMgr = m_pUser->GetCardMgr();
	if (!CardMgr.Get(i64CardId)) {//��Ƥ����δ֪�Ŀ��ƴ���
		return false;
	}

	CardSkinMapIter iterCardSkin = m_mapByCardId.find(i64CardId);
	if (iterCardSkin!=m_mapByCardId.end()) {
		Log( "������Ƥ���Ŀ����Ѿ����ϱ��Ƥ����\n");
		return false;
	}

	m_mapByCardId.insert({ i64CardId ,m_mapBySkinId[i64SkinId] });
	return true;
	
}
bool CSkinMgr::Del(long long int i64SkinId) {
	/*����Ƥ��Idɾ��Ƥ��*/
	if (i64SkinId <= 0) {
		Log( "Ƥ��ID���Ϸ�\n");
		return false;
	}
	CSkin* pSkin = nullptr;
	SkinMapIter iterSkin = m_mapBySkinId.find(i64SkinId);
	if(iterSkin !=m_mapBySkinId.end())
		pSkin = iterSkin->second;
	if (!pSkin) {
		Log("ɾ��ʧ�ܣ������û������IDΪ��"+to_string( i64SkinId )+" ��Ƥ��\n");
		return false;
	}

	const long long int i64CardId = pSkin->GetCardId();
	if (i64CardId != 0) {
		m_mapByCardId.erase(i64CardId);
	}

	const unsigned int unSkinType = pSkin->GetSkinType();
	m_setBySkinType.erase(unSkinType);
	m_mapBySkinId.erase(i64SkinId);
	bool ret = pSkin->Delete();
	delete pSkin;
	return ret;
}
void CSkinMgr::DeBug_DelAll() {
	/*ɾ�����е�Ƥ��*/
	m_mapByCardId.clear();
	m_setBySkinType.clear();
	for(auto &iter:m_mapBySkinId) {
		CSkin* pSkin = iter.second;
		if (!pSkin) {
			continue;
		}
		if (!pSkin->Delete()) {
			Log("����ɾ��ʧ��\n");
		}
		delete pSkin;
	}
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
			Log( "Ƥ�����Ͳ�����\n" );
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
		Log( "�û���Ƥ��������ʧ����\n" );
		return false;
	}

	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		Log( "û���ҵ�Ҫ����Ƥ���Ŀ���\n" );
		return false;
	}

	CardSkinMapIter iterCardSkin = m_mapByCardId.find(i64CardId);
	if (iterCardSkin!=m_mapByCardId.end()) {
		Log("�ÿ����Ѿ������˱��Ƥ����\n");
		return false;
	}

	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		Log("û���ҵ�Ҫ������Ƥ��");
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
		Log("�û���Ƥ��������ʧ����\n");
		return false;
	}

	CCardMgr& CardMgr = m_pUser->GetCardMgr();
	CCard* pCard = CardMgr.Get(i64CardId);
	if (!pCard) {
		Log("û���ҵ�Ҫ����Ƥ���Ŀ���\n");
		return false;
	}

	CardSkinMapIter iterCardSkin = m_mapByCardId.find(i64CardId);
	if (iterCardSkin==m_mapByCardId.end()) {
		Log("�ÿ��Ʊ���û��Ƥ��\n");
		return false;
	}

	CSkin* pSkin = Get(i64SkinId);
	if (!pSkin) {
		Log("û���ҵ�Ҫ���µ�Ƥ��\n");
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
	m_mapByCardId.clear();
	m_setBySkinType.clear();
	for (auto& iter:m_mapBySkinId) {
		delete iter.second;//���ͷ��ڴ�
	}
	m_mapBySkinId.clear();
}
