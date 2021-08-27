#pragma once
#ifndef _CARDTYPEMGR_H_
#define _CARDTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardType.h"
class CCardTypeMgr
{
public:
	CCardTypeMgr();/*���캯��*/
	~CCardTypeMgr();/*��������*/

	const CCardType* Get(const unsigned int unType);/*���ݿ������ͻ�ȡ����*/

	void PrintAll();/*��ӡ��ʾ���п�������*/
	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/

	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
private:

	using CardTypeMap = std::map<unsigned int,const CCardType*>;
	using CardTypeMapIter = CardTypeMap::iterator;
	CardTypeMap m_mapByType;/*ÿһ��������Ŀǰ���еĿ�������*/
};
extern CCardTypeMgr g_CardTypeMgr;
#endif //!_CARDTYPEMGR_H_
