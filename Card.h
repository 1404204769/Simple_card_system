#pragma once
#ifndef _CARD_H_
#define _CARD_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "CardType.h"
#include "SkinMgr.h"
#include "CardLevAttrTypeMgr.h"
class CCard
{
public:
	CCard();/*���캯��*/
	~CCard();/*��������*/

	bool CreateNewCard(const long long int i64UserId,const CCardType* pCardType);/*����һ���µĿ���*/
	bool CreateFromDB(const mysqlpp::Row& row, const CCardType* pCardType);/*�����ݿ��м��ؿ���*/

	const std::string& GetName() const;/*��ȡm_strName*/
	unsigned int GetLev() const;/*��ȡm_unLev*/
	unsigned int GetCardType() const;/*��ȡm_unCardType*/
	long long int GetCardId() const;/*��ȡm_i64CardId*/
	long long int GetUserId() const;/*��ȡm_i64UserId*/
	long long int GetSkinId() const;/*��ȡm_i64SkinId*/
	long long int GetExp() const;/*��ȡm_i64Exp*/
	const CCardType& GetCardTypeData() const;/*��ȡ��Ӧ�Ŀ���������ϸ����*/
	const CCardLevAttrType& GetCardLevAttrTypeData() const;/*��ȡ��Ӧ���Ƶȼ��ļӳ�����*/
	
	void SetSkinId(long long int i64SkinId);/*���ô�����Ƥ��*/

	bool LevelUp(unsigned int unLev);/*�������Ƶȼ��������ӳ�����*/

	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/

private:
	bool Update();/*�������ݿ�*/
	bool Insert(long long int& i64CardId_Out);/*��Card���ݲ������ݿ�*/

	bool SetLevel(unsigned int unLev);/*����Level�ȼ�*/

	unsigned int  m_unCardType = 0, m_unLev = 0;
	long long int m_i64CardId = 0, m_i64UserId = 0,m_i64SkinId = 0, m_i64Exp = 0;
	std::string m_strName;   
	const CCardType* m_pCardType = nullptr;
	const CCardLevAttrType* m_pCardLevAttrType = nullptr;
};
#endif //!_CARD_H_
