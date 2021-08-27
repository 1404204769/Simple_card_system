#pragma once
#ifndef _SKINTYPEMGR_H_
#define _SKINTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "SkinType.h"
class CSkinTypeMgr
{
public:
	CSkinTypeMgr();/*���캯��*/
	~CSkinTypeMgr();/*��������*/

	const CSkinType* Get(const unsigned int unId);/*���ݿ������ͻ�ȡ����*/

	void PrintAll();/*��ӡ��ʾ���п�������*/
	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/

	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/
private:

	using SkinTypeMap = std::map<unsigned int, const CSkinType*>;
	using SkinTypeMapIter = SkinTypeMap::iterator;
	SkinTypeMap m_mapSkinType;/*ÿһ��������Ŀǰ���еĿ�������*/
};
extern CSkinTypeMgr g_SkinTypeMgr;
#endif //!_SKINTYPEMGR_H_
