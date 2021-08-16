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

	CSkin* Get(const long long int i64SkinId);/*���ݿ������ƻ�ȡ��������*/

	bool Add(const long long int i64UserId, const CSkinType* pSkinType);/*����һ��ָ�����͵Ŀ���*/
	bool Del(long long int i64SkinId);/*������ҿ���������ɾ������*/
	bool DelAll();/*ɾ�����е�Ƥ��*/
	void PrintAll() const;/*��ӡ��ʾӵ�е�����Ƥ���Ƶ�����*/
	bool Wear(const long long int i64CardId, const long long int i64SkinId);/*ָ��ĳ�ſ��ƴ���ĳ��Ƥ��*/
	bool Drop(const long long int i64CardId, const long long int i64SkinId);/*ָ��ĳ�ſ�������ĳ��Ƥ��*/

private:

	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	CUser* m_pUser = nullptr;

	using SkinMap = std::map<long long int, CSkin*>;
	using SkinMapIter = SkinMap::iterator;
	SkinMap m_mapBySkinId;/*ÿһ��������Ŀǰ���еĿ�������*/
};
#endif //!_SKINMGR_H_
