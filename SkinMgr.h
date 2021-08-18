#pragma once
#ifndef _SKINMGR_H_
#define _SKINMGR_H_
#include "DB.h"
#include "Skin.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "SkinTypeMgr.h"
class CUser;
class CCardMgr;
class CSkinMgr
{
public:
	CSkinMgr();/*���캯��*/
	~CSkinMgr();/*��������*/

	bool Init(CUser* pUser);/*�����ڲ��������ݿ��ȡ����*/

	CSkin* Get(const long long int i64SkinId);/*����Ƥ��ID��ȡƤ������*/
	CSkin* GetWearing(const long long int i64CardId);/*����Ƥ��ID��ȡƤ������*/

	bool AddNew(const CSkinType* pSkinType);/*����һ��ָ�����͵�Ƥ��*/

	void DeBug_DelAll();/*ɾ�����е�Ƥ��*/
	void PrintAll();/*��ӡ��ʾӵ�е�����Ƥ���Ƶ�����*/
	bool Wear(const long long int i64CardId, const long long int i64SkinId);/*ָ��ĳ�ſ��ƴ���ĳ��Ƥ��*/
	bool Drop(const long long int i64CardId, const long long int i64SkinId);/*ָ��ĳ�ſ�������ĳ��Ƥ��*/

private:
	bool Add(CSkin* pSkin);/*�������Ӳ�����choice=1����������Ƥ������,choice=2:������Ƥ��*/
	bool Del(long long int i64SkinId);/*�������Ƥ��ID��ɾ������*/
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	CUser* m_pUser = nullptr;
	using SkinTypeSet = std::set<unsigned int>;
	SkinTypeSet m_setBySkinType;

	using CardSkinMap = std::map<long long int, CSkin*>;
	using CardSkinMapIter = CardSkinMap::iterator;
	CardSkinMap m_mapByCardId;/*ÿһ��������Ŀǰ�����ƴ�����Ƥ��*/

	using SkinMap = std::map<long long int, CSkin*>;
	using SkinMapIter = SkinMap::iterator;
	SkinMap m_mapBySkinId;/*ÿһ��������Ŀǰ���еĿ�������*/
};
#endif //!_SKINMGR_H_
