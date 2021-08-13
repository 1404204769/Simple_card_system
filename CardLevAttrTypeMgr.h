#pragma once
#ifndef _CARDLEVATTRTYPEMGR_H_
#define _CARDLEVATTRTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardLevAttrType.h"
class CCardLevAttrTypeMgr
{
public:
	CCardLevAttrTypeMgr();/*���캯��*/
	~CCardLevAttrTypeMgr();/*��������*/

	const CCardLevAttrType* Get(const unsigned int unLev);/*���ݿ���Lev��ȡ����*/

	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardLevAttrTypeMgr�����IDΪ0��˵�����ݿ����޴�����*/

private:
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	using CardLevAttrTypeMap = std::map<unsigned int, const CCardLevAttrType*>;
	using CardLevAttrTypeMapIter = CardLevAttrTypeMap::iterator;
	CardLevAttrTypeMap m_mapByLev;/*ÿһ��������Ŀǰ���еĿ�������*/
};
extern CCardLevAttrTypeMgr g_CardLevAttrTypeMgr;
#endif //!_CARDLEVATTRTYPEMGR_H_
