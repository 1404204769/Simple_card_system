#pragma once
#ifndef _CARDRANKTYPEMGR_H_
#define _CARDRANKTYPEMGR_H_
#include "DB.h"
#include <string>
#include <mysql++.h>
#include "CardRankType.h"
class CCardRankTypeMgr{
public:
	CCardRankTypeMgr();/*���캯��*/
	~CCardRankTypeMgr();/*��������*/

	const CCardRankType* Get(const unsigned int unId);/*���ݿ��ƽ׼���ȡ����*/

	bool Init();/*����CDB��������ָ������,���ز�ѯ�����������õ�CCardLevAttrTypeMgr�����IDΪ0��˵�����ݿ����޴�����*/

	bool Debug_PrintAll();/*��ӡ��ʾ��̬����*/

private:
	void Free();/*�����������е��ã��ͷŻ����ڴ��е����ݣ���ֹ�ڴ�й©�Լ����ݶ�ʧ*/

	using CardRankTypeMap = std::map<unsigned int, const CCardRankType*>;
	using CardRankTypeMapIter = CardRankTypeMap::iterator;
	CardRankTypeMap m_mapById;/*ÿһ��������Ŀǰ���еĿ�������*/
};
extern CCardRankTypeMgr g_CardRankTypeMgr;
#endif //!_CARDRANKTYPEMGR_H_
