#pragma once
#ifndef _CARDMGR_H_
#define _CARDMGR_H_
#include "DB.h"
#include "Card.h"
#include <vector>
#include <string>
#include <sstream>
#include <mysql++.h>
#include "CardTypeMgr.h"
extern CDB g_DB;
extern CCardTypeMgr g_CardTypeMgr;
class CCardMgr
{
public:
	CCardMgr();/*���캯��*/
	~CCardMgr();/*��������*/
	bool Init(long long int _i64UserId);/*�����ڲ��������ݿ��ȡ����*/
	CCard* GetCardByCardId(long long int _i64CardId);/*���ݿ������ƻ�ȡ��������*/
	void PrintAllCard();/*��ӡ��ʾӵ�е����п��Ƶ�����*/
	bool AddCard(long long int _i64UserId,const CCardType* _pCardType);/*����һ��ָ�����͵Ŀ���*/
	bool DelCardById(long long int _i64CardId);/*������ҿ���������ɾ������*/
	bool DelAllCard();/*ɾ�����еĿ���*/
private:
	bool Insert(long long int& _i64CardId, const long long int _i64UserId, const unsigned int _unCardType, const std::string& _strName, const long long int _i64Exp, const unsigned int _unLev);/*��Card���ݲ������ݿ�*/
	bool Delete(CCard& Card);/*��Card���ݴ����ݿ�ɾ��*/
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
	using MapByCardId = std::map<long long int, CCard*>;
	using MapByCardIdIter = MapByCardId::iterator;
	MapByCardId m_mapByCardId;/*ÿһ��������Ŀǰ���еĿ�������*/
	std::vector< CCard*> m_vecDel;
};

#endif //!_CARDMGR_H_
