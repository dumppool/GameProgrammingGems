// MyHLA helper library
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "StdAfx.h"
#include "message.h"

/** reads from message buffer */
bool CMessage::Read(void* data,int dataLength)
{
	if(m_cursor+dataLength>GetLength())
		return false;

	memcpy(data,&m_buffer[m_cursor],dataLength);
	m_cursor+=dataLength;

	return true;
}

/** writes to message buffer */
void CMessage::Write(const void* data,int dataLength)
{
	m_buffer.resize(GetLength()+dataLength); // TODO: make this code more faster!
	memcpy(&m_buffer[m_cursor],data,dataLength);
	m_cursor+=dataLength;
}

CMessage::CMessage()
{
	m_cursor=0;
}

CMessage::~CMessage()
{
}

/** sets read/write position (cursor) to a specified one. */
void CMessage::SetCursor(int cursor)
{
	m_cursor=cursor;
}

unsigned char* CMessage::GetBuffer()
{
	if(m_buffer.size()==0)
		return NULL;
	return &m_buffer[0];
}
