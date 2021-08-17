#pragma once
#ifndef _SKIN_H_
#define _SKIN_H_
#include "DB.h"
#include <string>
#include <assert.h>
#include <mysql++.h>
#include "SkinType.h"
class CCard;
class CSkin
{
public:
	CSkin();/*���캯��*/
	~CSkin();/*��������*/

	bool CreateNewSkin(const long long int i64UserId, const CSkinType* pSkinType);/*����һ���µĿ���*/
	bool CreateFromDB(const mysqlpp::Row& row, const CSkinType* pSkinType);/*�����ݿ��м��ؿ���*/

	unsigned int GetSkinType() const;/*��ȡm_unSkinType*/
	long long int GetCardId() const;/*��ȡm_i64CardId*/
	long long int GetSkinId() const;/*��ȡm_i64SkinId*/
	long long int GetUserId() const;/*��ȡm_i64UserId*/

	bool IsWear()const;/*�鿴��Ƥ���Ƿ񱻴���*/
	bool Wear(CCard& Card);/*����ĳ��Ƥ��*/
	bool Drop(CCard& Card);/*����ĳ��Ƥ��*/


	bool Delete();/*��Card���ݴ����ݿ�ɾ��*/
private:
	bool Insert(long long int& i64SkinId_Out);/*��Card���ݲ������ݿ�*/
	bool Update();/*�����µ����ݸ��µ����ݿ�*/
	unsigned int  m_unSkinType = 0;
	long long int m_i64SkinId = 0, m_i64UserId = 0,m_i64CardId=0;
};
#endif //!_SKIN_H_
