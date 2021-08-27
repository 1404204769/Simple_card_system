#pragma once
#ifndef _EQUIPMGR_H_
#define _EQUIPMGR_H_
#include "DB.h"
#include "Equip.h"
#include <vector>
#include <string>
#include <mysql++.h>
#include "EquipTypeMgr.h"
class CUser;
class CCardMgr;
class CEquipMgr
{
public:
	CEquipMgr();/*���캯��*/
	~CEquipMgr();/*��������*/

	bool Init(CUser* pUser);/*�����ڲ��������ݿ��ȡ����*/
	bool IsInit();/*�����Ƿ��ʼ��*/

	CEquip* Get(const long long int i64EquipId);/*����װ��ID��ȡƤ������*/

	bool AddNew(const CEquipType* pEquipType);/*����һ��ָ�����͵�װ��*/
	bool DelExist(const long long int i64EquipId);/*ɾ��һ�����е�װ��*/
	void PrintAll();/*��ӡ��ʾӵ�е�����װ��������*/
	long long int GetWearCardId(const long long int i64EquipId);/*��ѯװ�������ſ��ƴ���*/

	bool AddEquipCardMap(const long long int i64EquipId, const long long int i64CardId);/*��������������Ӳ���*/
	bool DelEquipCardMap(const long long int i64EquipId);/*�����������ɾ������*/
private:
	bool Add(CEquip* pEquip);/*�������Ӳ���*/
	bool Del(const long long int i64EquipId);/*����װ��ID��ɾ������*/
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	CUser* m_pUser = nullptr;
	bool m_bInit = false;

	using EquipCardMap = std::map<long long int,long long int>;//EquipID��Ϊkey CardId��Ϊvalue
	using EquipCardMapIter = EquipCardMap::iterator;
	EquipCardMap m_mapEquipCard;/*ÿһ��������Ŀǰ�����ƴ�����װ��*/

	using EquipMap = std::map<long long int, CEquip*>;
	using EquipMapIter = EquipMap::iterator;
	EquipMap m_map;/*ÿһ��������Ŀǰ���е�װ��*/
};
#endif //!_EQUIPMGR_H_
