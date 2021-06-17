// -----------------------------------------------------------
// KdTreeDemo - memory.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_MEMORY_H
#define I_MEMORY_H

#define MAXDYNAMIC 100

namespace KdTreeDemo {

class ObjectList;
class KdTreeNode;
struct EBox;
class EBoxList;
class MManager
{
public:
	MManager();
	static void Init( unsigned int a_Size );
	static void PrepForDynamics();
	static void PrepRebuild();
	static KdTreeNode* NewKdTreeNodePair();
	static KdTreeNode* NewKdTreeNodePairD();
	static ObjectList* NewObjectList();
	static void FreeObjectList( ObjectList* a_List );
	static EBox* NewEBox();
	static EBoxList* NewEBoxList();
private:
	static ObjectList* m_OList, *m_OPtr;
	static EBox* m_EPtr, *m_OEPtr;
	static EBoxList* m_ELPtr, *m_OELPtr;
	static unsigned int m_EFree, m_EReq, m_OReq, m_KdReq, m_ELFree, m_ELReq, m_KdFree;
public:
	static KdTreeNode* m_KdPtr, *m_OKdPtr;
	static KdTreeNode* m_KdA, *m_OKdA;
};

}; // namespace Raytracer

#endif
