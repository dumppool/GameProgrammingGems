#pragma once

#include "message.h"

template<class type>
class CStaticBehaviorMember
{
	type m_value_INTERNAL;
public:
	inline void Set(type value)
	{
		m_value_INTERNAL=value;
	}
	inline type Get()
	{
		return m_value_INTERNAL;
	}

	// does nothing, but it is needed because it is called by SWD compiler output.
	// \return always false
	inline bool GatherTheChangeToMessage(HLAMemberID memberID, CMessage &outputMessage,Protocol &outProtocol)
	{
		outProtocol=Protocol_Undefined;
		return false;
	}

	inline void ClearTheChange() 
	{
	}

	inline bool IsChanged()
	{
		return false;
	}

	// does nothing, but it is needed because it is called by SWD compiler output.
	inline void GatherAllToMessage(HLAMemberID memberID, CMessage &outputMessage)
	{
	}
};

/** a template class for conditional behavior member
*/
template<class type>
class CConditionalBehaviorMember
{
	type m_value_INTERNAL;
	bool m_valueChanged_INTERNAL;
public:
	inline CConditionalBehaviorMember()
	{
		m_valueChanged_INTERNAL=false;
	}
	inline void Set(type value)
	{
		if(value!=m_value_INTERNAL)
		{
			// A variable whose 'changed' 
			// is true will be broadcasted soon.
			m_valueChanged_INTERNAL=true;
			m_value_INTERNAL=value;
		}
	}
	inline type Get()
	{
		return m_value_INTERNAL;
	}

	// append the value to the message if it has been changed.
	// \return true if the value is written to the message.
	inline bool GatherTheChangeToMessage(HLAMemberID memberID, CMessage &outputMessage,Protocol &outProtocol)
	{
		outProtocol=Protocol_Undefined;
		if(m_valueChanged_INTERNAL)
		{
			Message_Write(outputMessage,memberID);
			Message_Write(outputMessage,m_value_INTERNAL);
			outProtocol=Protocol_ReliableSend;
			
			return true;
		}

		return false;
	}

	inline void ClearTheChange()
	{
		m_valueChanged_INTERNAL=false;
	}

	inline bool IsChanged()
	{
		return m_valueChanged_INTERNAL;
	};

	// does nothing, but it is needed because it is called by SWD compiler output.
	inline void GatherAllToMessage(HLAMemberID memberID, CMessage &outputMessage)
	{
		Message_Write(outputMessage,memberID);
		Message_Write(outputMessage,m_value_INTERNAL);
	}
};

