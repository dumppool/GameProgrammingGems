// SWD compiler backend
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "StdAfx.h"
#include ".\backend.h"

extern CString GetFileNameOnly(CString originalFileName);

void CBackend::WritePragmaOnce(ostream &os)
{
	os<<"#pragma once\n";
}

void CBackend::WriteClassAndMemberIDs(ostream &os,CSpecification* spec)
{
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		CString fmt;
		fmt.Format("const HLAClassID HlaId_%s = %s;\n",synchClass.m_name,synchClass.m_ID);
		os<<fmt;

		for(CSynchClass::Members::iterator member0=synchClass.m_members.begin();
			member0!=synchClass.m_members.end();member0++)
		{
			CMember &member=**member0;
			fmt.Format("const HLAMemberID HlaId_%s_%s = %d;\n",synchClass.m_name,member.m_name,member.m_ID);
			os<<fmt;
		}
	}
}

void CBackend::WriteSynchClassDecls_S(ostream &os,CSpecification* spec)
{
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		WriteSynchClassDecl_S(os,&synchClass);
	}
}

void CBackend::WriteSynchClassDecls_C(ostream &os,CSpecification* spec)
{
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		WriteSynchClassDecl_C(os,&synchClass);
		os<<";\n";
	}
}

// Writes a synch class declaration
void CBackend::WriteSynchClassDecl_S(ostream &os,CSynchClass* synchClass)
{
	CString fmt;
	fmt.Format("class %s_S:public SynchEntity_S\n{\n",synchClass->m_name); os<<fmt;
	for(CSynchClass::Members::iterator iMember=synchClass->m_members.begin();iMember!=synchClass->m_members.end();iMember++)
	{
		RefCount<CMember> member=*iMember;
		WriteMemberDecl_S(os,member);
	}
	os<<"public:\n";
	os<<"\tvirtual bool GatherTheChangeToMessage(CMessage &outputMessage,Protocol &outProtocol);\n";
	os<<"\tvirtual void GatherAllToMessage( CMessage &outputMessage );\n";
	os<<"\tvirtual HLAClassID GetClassID();\n";
	os<<"\tvirtual bool HasChanges();\n";
	os<<"\tvirtual void ClearTheChanges();\n";
	os<<"};\n\n";
}

// Writes a synch class declaration
void CBackend::WriteSynchClassDecl_C(ostream &os,CSynchClass* synchClass)
{
	CString fmt;
	fmt.Format("class %s_C:public SynchEntity_C\n{\npublic:\n",synchClass->m_name); os<<fmt;
	for(CSynchClass::Members::iterator iMember=synchClass->m_members.begin();iMember!=synchClass->m_members.end();iMember++)
	{
		RefCount<CMember> member=*iMember;
		WriteMemberDecl_C(os,member);
	}
	os<<"\tvirtual void UpdateFromMessage(CMessage& msg);\n";
	os<<"};\n\n";

	os<<"class RuntimeDelegate_C:public IHlaRuntimeDelegate_C\n";
	os<<"{\n";
	os<<"public:\n";
	os<<"\tvirtual SynchEntity_C* CreateSynchEntityByClassID(HLAClassID classID);\n";
	os<<"};\n";
}

void CBackend::WriteSynchClassImpls_S(ostream &os,CSpecification* spec)
{
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		WriteSynchClassImpl_S(os,synchClass);
	}
}

void CBackend::WriteSynchClassImpls_C(ostream &os,CSpecification* spec)
{
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		WriteSynchClassImpl_C(os,synchClass);
	}
}

void CBackend::WriteMemberDecl_S( ostream &os,CMember* member )
{
	CString fmt;
	fmt.Format("\tDECLARE_HLA_MEMBER(%s,%s,%s);\n",member->m_behavior->GetTemplateClassName(),member->m_type,member->m_name);
	os<<fmt;
}

void CBackend::WriteMemberDecl_C( ostream &os,CMember* member )
{
	CString fmt;
	fmt.Format("\t%s %s;\n",member->m_type,member->m_name);
	os<<fmt;
}

void CBackend::WriteNamespaceStart(ostream &os, LPCTSTR namespaceName )
{
	CString fmt;
	fmt.Format("namespace %s\n{\n",namespaceName);
	os<<fmt;
}

void CBackend::WriteNamespaceEnd( ostream &os )
{
	os<<"}\n";
}

void CBackend::WriteIncludeHeader( ostream &os,LPCTSTR fileName,LPCTSTR postfix )
{
	CString t;
	t.Format("\n#include \"%s%s.h\"\n\n",GetFileNameOnly(fileName),postfix);
	os<<t; 
}

void CBackend::WriteSynchClassImpl_S( ostream &os,CSynchClass& synchClass )
{
	CString t;

	t.Format("\tbool %s_S::GatherTheChangeToMessage( CMessage &outputMessage ,Protocol &outProtocol)\n",synchClass.m_name); os<<t;
	t.Format("\t{\n"); os<<t;
	t.Format("\t\toutProtocol=Protocol_Undefined;\n"); os<<t;
	t.Format("\t\tbool ret=false;\n"); os<<t;
	t.Format("\t\tProtocol prot;\n"); os<<t;

	t.Format("\t\t// for each member, gather the change and append to the message and retrieve the appropriate protocol\n"); os<<t;
	for(CSynchClass::Members::iterator param0=synchClass.m_members.begin();param0!=synchClass.m_members.end();param0++)
	{
		CMember& param=**param0;
		t.Format("\t\tret |= m_%s_INTERNAL.GatherTheChangeToMessage(HlaId_%s_%s,outputMessage,prot);\n",param.m_name,synchClass.m_name,param.m_name); os<<t;
		t.Format("\t\tif(ret)\n"); os<<t;
		t.Format("\t\t{\n"); os<<t;
		t.Format("\t\t\toutProtocol=CombineProtocol(outProtocol,prot);\n"); os<<t;
		t.Format("\t\t}\n"); os<<t;
	}

	t.Format("\t\treturn ret;\n"); os<<t;
	t.Format("\t}\n"); os<<t;

	t.Format("\tvoid %s_S::GatherAllToMessage( CMessage &outputMessage )\n",synchClass.m_name); os<<t;
	t.Format("\t{\n"); os<<t;
	for(CSynchClass::Members::iterator param0=synchClass.m_members.begin();param0!=synchClass.m_members.end();param0++)
	{
		CMember& param=**param0;
		t.Format("\t\tm_%s_INTERNAL.GatherAllToMessage(HlaId_%s_%s,outputMessage);\n",param.m_name,synchClass.m_name,param.m_name); os<<t;
	}
	t.Format("\t}\n"); os<<t;

	t.Format("\tHLAClassID %s_S::GetClassID()\n",synchClass.m_name); os<<t;
	t.Format("\t{\n"); os<<t;
	t.Format("\t\treturn HlaId_%s;\n",synchClass.m_name); os<<t;
	t.Format("\t}\n"); os<<t;

	t.Format("\tbool %s_S::HasChanges()\n",synchClass.m_name); os<<t;
	t.Format("\t{\n"); os<<t;
	t.Format("\t\t// for each member variable...\n"); os<<t;
	for(CSynchClass::Members::iterator param0=synchClass.m_members.begin();param0!=synchClass.m_members.end();param0++)
	{
		CMember& param=**param0;
		t.Format("\t\tif(m_%s_INTERNAL.IsChanged()) return true;\n",param.m_name); os<<t;
	}

	t.Format("\t\treturn false;\n"); os<<t;
	t.Format("\t}\n"); os<<t;

	t.Format("\tvoid %s_S::ClearTheChanges()\n",synchClass.m_name); os<<t;
	t.Format("\t{\n"); os<<t;
	t.Format("\t\t// for each member variable...\n"); os<<t;
	for(CSynchClass::Members::iterator param0=synchClass.m_members.begin();param0!=synchClass.m_members.end();param0++)
	{
		CMember& param=**param0;
		t.Format("\t\tm_%s_INTERNAL.ClearTheChange();\n",param.m_name); os<<t;
	}
	t.Format("\t}\n"); os<<t;
}

void CBackend::WriteSynchClassImpl_C( ostream &os,CSynchClass& synchClass )
{
	CString t;

	t.Format("void %s_C::UpdateFromMessage( CMessage& msg )\n",synchClass.m_name ); os<<t;
	t.Format("{\n" ); os<<t;
	t.Format("\tHLAMemberID memberID;\n" ); os<<t;
	t.Format("\twhile(Message_Read(msg,memberID))\n" ); os<<t;
	t.Format("\t{\n" ); os<<t;
	t.Format("\t\tswitch(memberID)\n" ); os<<t;
	t.Format("\t\t{\n" ); os<<t;
	for(CSynchClass::Members::iterator param0=synchClass.m_members.begin();param0!=synchClass.m_members.end();param0++)
	{
		CMember& param=**param0;
		t.Format("\t\tcase HlaId_%s_%s:\n",synchClass.m_name,param.m_name ); os<<t;
		t.Format("\t\t\tMessage_Read(msg,%s);\n" ,param.m_name); os<<t;
		t.Format("\t\t\tbreak;\n" ); os<<t;
	}
	t.Format("\t\t}\n" ); os<<t;
	t.Format("\t}\n" ); os<<t;
	t.Format("}\n" ); os<<t;
}

void CBackend::WriteRuntimeDelegate_C( ostream &os,CSpecification* spec )
{
	CString t;
	
	t.Format("SynchEntity_C* RuntimeDelegate_C::CreateSynchEntityByClassID( HLAClassID classID )\n" ); os<<t;
	t.Format("{\n" ); os<<t;
	t.Format("\tswitch(classID)\n" ); os<<t;
	t.Format("\t{\n" ); os<<t;
	for(CSpecification::SynchClasses::iterator synchClass0=spec->m_synchClasses.begin();
		synchClass0!=spec->m_synchClasses.end();
		synchClass0++)
	{
		CSynchClass& synchClass=*synchClass0;
		t.Format("\tcase HlaId_%s:\n",synchClass.m_name ); os<<t;
		t.Format("\t\treturn new %s_C;\n",synchClass.m_name ); os<<t;
	}
	t.Format("\t}\n" ); os<<t;
	t.Format("\treturn NULL;\n" ); os<<t;
	t.Format("}\n" ); os<<t;
}