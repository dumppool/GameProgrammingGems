// -----------------------------------------------------------
// KdTreeDemo - memory.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "template.h"
#include "scene.h"
#include "kdtree.h"
#include "memory.h"

using namespace KdTreeDemo;

_declspec(align(16)) 

ObjectList* MManager::m_OList, *MManager::m_OPtr;
KdTreeNode* MManager::m_KdPtr, *MManager::m_OKdPtr;
KdTreeNode* MManager::m_KdA, *MManager::m_OKdA;
EBox* MManager::m_EPtr, *MManager::m_OEPtr;
EBoxList* MManager::m_ELPtr, *MManager::m_OELPtr;
unsigned int MManager::m_EFree, MManager::m_EReq, MManager::m_ELFree, MManager::m_ELReq;
unsigned int MManager::m_OReq, MManager::m_KdFree, MManager::m_KdReq;

MManager::MManager()
{
	m_EPtr = 0;
	m_ELPtr = 0;
	m_EFree = m_EReq = 0;
	m_ELFree = m_ELReq = 0;
	m_OList = m_OPtr = 0;
	m_OReq = 0;
	m_KdPtr = 0;
	m_KdA = 0;
	m_KdFree = m_KdReq = 0;
	
}

void MManager::PrepForDynamics()
{
	m_KdReq = MAXDYNAMIC * 40;
	m_OReq = MAXDYNAMIC * 40;
	m_EReq = MAXDYNAMIC * 40;
	m_ELReq = MAXDYNAMIC * 40;
	unsigned long addr = (unsigned long)(char*)(new KdTreeNode[m_KdReq + 4]);
	m_KdPtr = m_OKdPtr = (KdTreeNode*)((addr + 32) & (0xffffffff - 31));
	addr = (unsigned long)(char*)(new KdTreeNode[m_KdReq + 4]);
	m_KdA = m_OKdA = (KdTreeNode*)((addr + 32) & (0xffffffff - 31));
	addr = (unsigned long)(char*)(new ObjectList[m_OReq + 8]);
	m_OList = m_OPtr = (ObjectList*)((addr + 32) & (0xffffffff - 31));
	addr = (unsigned long)(char*)(new EBox[m_EReq + 4]);
	m_EPtr = m_OEPtr = (EBox*)((addr + 63) & (0xffffffff - 63));
	addr = (unsigned long)(char*)(new EBoxList[m_ELReq + 4]);
	m_ELPtr = m_OELPtr = (EBoxList*)((addr + 63) & (0xffffffff - 63));
}

void MManager::PrepRebuild()
{
	// reset all arrays
	m_KdPtr = m_OKdPtr;
	m_KdFree = MAXDYNAMIC * 40 - 4;
	m_KdA = m_OKdA;
	m_OList = m_OPtr;
	m_EPtr = m_OEPtr;
	m_EFree = MAXDYNAMIC * 40 - 4;
	m_ELPtr = m_OELPtr;
	m_ELFree = MAXDYNAMIC * 40 - 4;
	ObjectList* ptr = m_OList;
	for ( unsigned int i = 0; i < (MAXDYNAMIC * 40); i++, ptr++ ) ptr->SetNext( ptr + 1 );
	ptr->SetNext( 0 );
	memset( m_EPtr, 0, (MAXDYNAMIC * 40) * 64 );
}

KdTreeNode* MManager::NewKdTreeNodePair()
{ 
	if (m_KdFree == 0)
	{
		unsigned long addr = (unsigned long)(new KdTreeNode[32800]);
		m_KdPtr = (KdTreeNode*)((addr + 32) & (0xffffffff - 31));
		m_KdFree = 16384;
		m_KdReq += 16384;
	}
	unsigned long* tmp = (unsigned long*)m_KdPtr;
	tmp[1] = tmp[3] = 6;
	KdTreeNode* node = m_KdPtr;
	m_KdPtr += 2;
	m_KdFree--;
	return node;
}

KdTreeNode* MManager::NewKdTreeNodePairD()
{
	unsigned long* tmp = (unsigned long*)m_KdA;
	tmp[1] = tmp[3] = 6;
	KdTreeNode* node = m_KdA;
	m_KdA += 2;
	return node;
}

ObjectList* MManager::NewObjectList()
{
	ObjectList* retval;
	if (!m_OList)
	{
		unsigned long addr = (unsigned long)(new ObjectList[8200]);
		m_OList = m_OPtr = (ObjectList*)((addr + 32) & (0xffffffff - 31));
		ObjectList* ptr = m_OList;
		for ( unsigned int i = 0; i < 1020; i++, ptr++ ) ptr->SetNext( ptr + 1 );
		ptr->SetNext( 0 );
		m_OReq += 8192;
	}
	retval = m_OList;
	m_OList = m_OList->GetNext();
	retval->SetNext( 0 );
	retval->SetPrimitive( 0 );
	return retval;
}

void MManager::FreeObjectList( ObjectList* a_List )
{
	ObjectList* list = a_List;
	if (list)
	{
		while (list->GetNext()) list = list->GetNext();
		list->SetNext( m_OList );
		m_OList = a_List;
	}
}

EBox* MManager::NewEBox()
{
	if (m_EFree == 0)
	{
		unsigned long addr = (unsigned long)(char*)(new EBox[16400]);
		m_EPtr = (EBox*)((addr + 63) & (0xffffffff - 63));
		memset( m_EPtr, 0, 16384 * 64 );
		m_EFree = 16384;
		m_EReq += 16384;
	}
	m_EFree--;
	EBox* retval = m_EPtr;
	m_EPtr++;
	return retval;
}

EBoxList* MManager::NewEBoxList()
{
	if (m_ELFree == 0)
	{
		unsigned long addr = (unsigned long)(char*)(new EBoxList[16400]);
		m_ELPtr = (EBoxList*)((addr + 63) & (0xffffffff - 63));
		m_ELFree = 16384;
		m_ELReq += 16384;
	}
	m_ELFree--;
	EBoxList* retval = m_ELPtr;
	m_ELPtr++;
	return retval;
}