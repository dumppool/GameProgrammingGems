// MyHLA helper library header file
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#pragma once

#include <vector>
#include <string>

/** HLA message identifier
\todo you may change this type to what you want */
typedef unsigned char HLAMessageType;
typedef unsigned short HLAClassID; 
typedef unsigned short HLAMemberID;
typedef unsigned int HLAInstanceID;
typedef unsigned int HLAHostID;

const HLAMessageType HLAMessageType_None=0; 
const HLAMessageType HLAMessageType_Appear=1;			// send that an object appears
const HLAMessageType HLAMessageType_Disappear=2;			// send that an object disappears
const HLAMessageType HLAMessageType_ShowState=3;			// send the changed values of the modified member variables

const HLAInstanceID HLAInstanceID_None=0;
const HLAHostID HLAHostID_None=0;

enum Protocol 
{
	Protocol_Undefined, 
	Protocol_UnreliableSend, 
	Protocol_ReliableSend, 
};

/** an identifier indicating send-to target
\todo you may change this type to what you want */
typedef unsigned int HLASendTo;

/** additional send context identifier, for example, message protocol, asynchronous send notification.
\todo you may change this type to what you want */
typedef unsigned int HLAProtocol;

/** return values from synch class invocation.
\todo you may change this type to what you want */
typedef unsigned int HLAResult;

/** means, "synch class call successful" */
static const HLAResult HLAResult_Ok=0;

/** Simple message class.
CMessage works like a file, but it stores its data in a memory block. 
CMessage has 'cursor', which roles as a file pointer.

\todo This class may cause many memory fragmentations due to its internal
heap allocation. You may want to solve this problem by, for example, adding custom allocators or
allowing CMessage to receive external message buffer from your network engine. 

\todo This class does minimum error checking so you may add more code here. */
class CMessage
{
	vector<BYTE> m_buffer;
	int m_cursor;
public:
	CMessage();
	~CMessage();

	bool Read(void* data,int dataLength);
	void Write(const void* data,int dataLength);
	void SetCursor(int cursor);
	inline int GetCursor() { return m_cursor; }
	unsigned char* GetBuffer();
	inline int GetLength() { return (int)m_buffer.size(); }
};

// 
//These functions serialize a variable into CMessage object.
// 

inline void Message_Write(CMessage& m,const bool &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const unsigned char &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const char &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const unsigned short &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const short &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const unsigned int &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const int &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const unsigned long &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const long &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const float &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const double &val)
{
	m.Write(&val,sizeof(val));
}

inline void Message_Write(CMessage& m,const std::string &val)
{
	size_t length=val.size();
	Message_Write(m,length);
	m.Write(val.c_str(),(int)length);
}

inline bool Message_Read(CMessage& m,bool &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,unsigned char &val)
{
	return m.Read(&val,sizeof(val));
}

// 
//These functions deserialize a variable out of CMessage object.
// 

inline bool Message_Read(CMessage& m,char &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,unsigned short &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,short &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,unsigned int &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,int &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,unsigned long &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,long &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,float &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,double &val)
{
	return m.Read(&val,sizeof(val));
}

inline bool Message_Read(CMessage& m,std::string &val)
{
	size_t length;
	Message_Read(m,length);
	val.resize(length,0);
	m.Read(&val[0],(int)length);
}

inline bool Message_Read(CMessage& m,CPoint &value)
{
	return Message_Read(m,value.x) &&
		Message_Read(m,value.y);
}

inline void Message_Write(CMessage& m,const CPoint &value)
{
	Message_Write(m,value.x);
	Message_Write(m,value.y);
}

/** This function shall be called by MyHLA proxy. 
You should implement your own one. */
HLAResult HLA_Send(HLASendTo sendTo,HLAProtocol protocol,CMessage& m);
