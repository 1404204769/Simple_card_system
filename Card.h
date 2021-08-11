#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include <string>
#include <sstream>
#include <mysql++.h>
#include "DB.h"
#include "CardType.h"
extern CDB g_DB;
class CCard
{
public:
	CCard();/*���캯��*/
	~CCard();/*��������*/
	bool Init(const long long int _i64CardId,const long long int _i64UserId,const unsigned int _unCardType,const std::string& _strName,const long long int _i64Exp,const unsigned int _unLev, const CCardType* _pCardType);/*���ݻ�ȡ�����ݿ����ݳ�ʼ����̬��������*/
	std::string& GetName();/*��ȡm_strName*/
	unsigned int GetLev();/*��ȡm_unLev*/
	unsigned int GetCardType();/*��ȡm_unCardType*/
	unsigned int GetMark();/*��ȡm_unMark*/
	long long int GetCardId();/*��ȡm_i64CardId*/
	long long int GetUserId();/*��ȡm_i64UserId*/
	long long int GetExp();/*��ȡm_i64Exp*/
	bool SetMark(unsigned int _unMark);/*����ջ״̬ 1.�� 2.ɾ 3.�� 4.�ޱ仯*/
	const CCardType* GetCardTypeData();/*��ȡ��Ӧ�Ŀ���������ϸ����*/

private:
	bool Insert();/*��Card���ݲ������ݿ�*/
	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/
	bool Update();/*�������ݿ�*/
	unsigned int  m_unCardType = 0, m_unLev = 0,m_unMark=0;
	std::string m_strName = "";
	long long int m_i64CardId = 0, m_i64UserId = 0, m_i64Exp = 0;
	const CCardType* m_pCardType = nullptr;
};
#endif //!_CARD_H_
