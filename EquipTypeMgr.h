#pragma once
#ifndef _EQUIPTYPEMGR_H_
#define _EQUIPTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "EquipType.h"
class CEquipTypeMgr
{
public:
	CEquipTypeMgr();/*���캯��*/
	~CEquipTypeMgr();/*��������*/

	const CEquipType* Get(const unsigned int unId);/*����װ�����ͻ�ȡ����*/

	void PrintAll();/*��ӡ��ʾ����װ������*/
	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CUser�����IDΪ0��˵�����ݿ����޴�����*/

private:
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	using EquipTypeMap = std::map<unsigned int, const CEquipType*>;
	using EquipTypeMapIter = EquipTypeMap::iterator;
	EquipTypeMap m_mapEquipType;/*ÿһ��������Ŀǰ���е�װ������*/
};
extern CEquipTypeMgr g_EquipTypeMgr;
#endif //!_EQUIPTYPEMGR_H_
