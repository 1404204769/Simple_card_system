#pragma once
#ifndef _CARDTYPEMGR_H_
#define _CARDTYPEMGR_H_
#include "DB.h"
#include <string>
#include <sstream>
#include <mysql++.h>
#include "CardType.h"
extern CDB g_DB;
class CCardTypeMgr
{
public:
	CCardTypeMgr();/*���캯��*/
	~CCardTypeMgr();/*��������*/
	const CCardType* GetCardTypeByType(unsigned int _unType);/*���ݿ���ID��ȡ����*/
	void PrintAllCardType();/*��ӡ��ʾ���п�������*/
	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/
private:
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	using MapByType = std::map<unsigned int,const CCardType*>;
	using MapByTypeIter = MapByType::iterator;
	MapByType m_mapByType;/*ÿһ��������Ŀǰ���еĿ�������*/
};
#endif //!_CARDTYPEMGR_H_
