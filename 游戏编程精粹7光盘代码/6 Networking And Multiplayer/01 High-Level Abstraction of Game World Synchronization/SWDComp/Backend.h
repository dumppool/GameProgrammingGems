// SWD compiler backend 
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#pragma once

#include "Specification.h"

class CBackend
{
public:
	static void WriteMemberDecl_S(ostream &os,CMember* member);
	static void WriteMemberDecl_C(ostream &os,CMember* member);
	static void WritePragmaOnce(ostream &os);
	static void WriteClassAndMemberIDs(ostream &os,CSpecification* spec);
	static void WriteSynchClassDecls_S(ostream &os,CSpecification* spec);
	static void WriteSynchClassDecls_C(ostream &os,CSpecification* spec);
	static void WriteSynchClassDecl_S(ostream &os,CSynchClass* func);
	static void WriteSynchClassDecl_C(ostream &os,CSynchClass* func);
	static void WriteSynchClassImpls_S(ostream &os,CSpecification* spec);
	static void WriteSynchClassImpls_C(ostream &os,CSpecification* spec);
	static void WriteNamespaceStart(ostream &os,LPCTSTR namespaceName);
	static void WriteNamespaceEnd(ostream &os);
	static void WriteIncludeHeader( ostream &os,LPCTSTR fileName,LPCTSTR postfix );
	static void WriteSynchClassImpl_S(ostream &os,CSynchClass& synchClass);
	static void WriteSynchClassImpl_C(ostream &os,CSynchClass& synchClass);
	static void WriteRuntimeDelegate_C(ostream &os,CSpecification* spec);
};
