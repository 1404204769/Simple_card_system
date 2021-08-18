#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include "DB.h"
#include "Card.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "CardTypeMgr.h"
class CUser;
class CCardMgr
{
public:
	CCardMgr();/*���캯��*/
	~CCardMgr();/*��������*/

	bool Init(CUser* pUser);/*�����ڲ��������ݿ��ȡ����*/

	CCard* Get(const long long int i64CardId);/*���ݿ������ƻ�ȡ��������*/

	bool Add(const CCardType* pCardType);/*����һ��ָ�����͵Ŀ���*/
	bool Del(long long int i64CardId);/*������ҿ���������ɾ������*/
	bool DelAll();/*ɾ�����еĿ���*/
	void PrintAll();/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	bool IsInit()const;/*����Ƿ��ʼ��*/

private:
	
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	CUser* m_pUser = nullptr;
	bool m_bInit = false;

	using CardMap = std::map<long long int, CCard*>;
	using CardMapIter = CardMap::iterator;
	CardMap m_mapByCardId;/*ÿһ��������Ŀǰ���еĿ�������*/
};

#endif //!_CARDMGR_H_
