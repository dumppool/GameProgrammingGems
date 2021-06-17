// HLAComp.cpp : Defines the entry point for the console application.
//
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "stdafx.h"
#include "IncludeGeneratedFiles.h"
#include "backend.h"
#include <atlpath.h>

using namespace antlr;

CString GetSubstitutedFileName(CString originalFileName,const char* postfix,const char* extToSubsitute)
{
	char fullPath[_MAX_PATH];

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName,_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);
	strcat(fname,postfix);
	strcpy(ext,extToSubsitute);

	_makepath(fullPath,drive,dir,fname,ext);

	return CString(fullPath);
}

CString GetFileNameOnly(CString originalFileName)
{
	char fullPath[_MAX_PATH];

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName,_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);

	return fname;
}

void WriteCommonHFile( CString inputFileName, RefCount<CSpecification> spec )
{
	ofstream commonh;
	commonh.open(GetSubstitutedFileName(inputFileName,"_common","h"));
	CBackend::WritePragmaOnce(commonh);			
	CBackend::WriteNamespaceStart(commonh,spec->m_configurations.m_namespace);
	CBackend::WriteClassAndMemberIDs(commonh,spec);
	CBackend::WriteNamespaceEnd(commonh);
}

void WriteServerHFile( CString inputFileName, RefCount<CSpecification> spec )
{
	ofstream os;
	os.open(GetSubstitutedFileName(inputFileName,"_s","h"));
	CBackend::WritePragmaOnce(os);
	CBackend::WriteIncludeHeader(os,inputFileName,"_common");
	CBackend::WriteNamespaceStart(os,spec->m_configurations.m_namespace);
	CBackend::WriteSynchClassDecls_S(os,spec);
	CBackend::WriteNamespaceEnd(os);
}

void WriteClientHFile( CString inputFileName, RefCount<CSpecification> spec )
{
	ofstream os;
	os.open(GetSubstitutedFileName(inputFileName,"_c","h"));
	CBackend::WritePragmaOnce(os);
	CBackend::WriteIncludeHeader(os,inputFileName,"_common");
	CBackend::WriteNamespaceStart(os,spec->m_configurations.m_namespace);
	CBackend::WriteSynchClassDecls_C(os,spec);
	CBackend::WriteNamespaceEnd(os);
}

void WriteServerCppFile( CString inputFileName, RefCount<CSpecification> spec )
{
	ofstream os;
	os.open(GetSubstitutedFileName(inputFileName,"_s","cpp"));
	CBackend::WriteIncludeHeader(os,inputFileName,"_s");
	CBackend::WriteNamespaceStart(os,spec->m_configurations.m_namespace);
	CBackend::WriteSynchClassImpls_S(os,spec);			
	CBackend::WriteNamespaceEnd(os);
}

void WriteClientCppFile( CString inputFileName, RefCount<CSpecification> spec )
{
	ofstream os;
	os.open(GetSubstitutedFileName(inputFileName,"_c","cpp"));
	CBackend::WriteIncludeHeader(os,inputFileName,"_c");
	CBackend::WriteNamespaceStart(os,spec->m_configurations.m_namespace);
	CBackend::WriteSynchClassImpls_C(os,spec);			
	CBackend::WriteRuntimeDelegate_C(os,spec);
	CBackend::WriteNamespaceEnd(os);
}

int main(int argc, _TCHAR* argv[])
{

	if(argc==1)
	{
		cout<<"Synchronized World Definition compiler authored by Hyun-jik Bae(imays@paran.com)\n";
		cout<<"Command line options: (.SWD file name)\n";
		return -1;
	}

	try 
	{
		RefCount<CSpecification> spec; // parse information
		CString inputFileName=argv[1];

		// front-end: read file and parse it
		{
			ifstream fileIn; 

			fileIn.open(inputFileName);
			SWDLexer lexer(fileIn);
			SWDParser parser(lexer);
			spec=parser.specification(); 
		}

		// back-end: generate proxy and stub code

		WriteCommonHFile(inputFileName, spec);
		WriteServerHFile(inputFileName, spec);
		WriteServerCppFile(inputFileName, spec);


		WriteClientHFile(inputFileName, spec);

		WriteClientCppFile(inputFileName, spec);

	}
	catch( ANTLRException& e )
	{
		cerr << "exception: " << e.getMessage() << endl;
		return -1;
	}
	catch(exception& e)
	{
		cerr << "exception: " << e.what() << endl;
		return -1;
	}
	return 0;
}

