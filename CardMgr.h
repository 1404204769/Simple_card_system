#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
#include "Card.h"
#include "CardTypeMgr.h"
extern CDB g_DB;
extern CCardTypeMgr g_CardTypeMgr;
class CCardMgr
{
public:
	CCardMgr();/*���캯��*/
	~CCardMgr();/*��������*/
	bool Init(long long int _i64UserId);/*�����ڲ��������ݿ��ȡ����*/
	CCard* GetCardByCardName(std::string _strName);/*���ݿ������ƻ�ȡ��������*/
	void PrintAllCard();/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	bool AddCard(long long int _i64UserId,const CCardType* _pCardType);/*����һ��ָ�����͵Ŀ���*/
	bool DelCardByName(std::string _strName);/*������ҿ���������ɾ������*/
	bool DelAllCard();/*ɾ�����еĿ���*/
private:
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	using MapByCardName = std::map<std::string, CCard*>;
	using MapByCardNameIter = MapByCardName::iterator;
	MapByCardName m_mapByCardName;/*ÿһ��������Ŀǰ���еĿ�������*/
};
#endif //!_CARDMGR_H_
