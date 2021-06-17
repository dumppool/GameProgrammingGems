#ifndef _NETWORK_TWEAKABLES_H_
#define _NETWORK_TWEAKABLES_H_

class NetworkTweakables
{
public:
	NetworkTweakables():
	  m_fReliablePacketResentTime(2.0f)
	{

	}

	static NetworkTweakables& GetInstance()
	{
		static NetworkTweakables sNetworkTweakables;
		return sNetworkTweakables;
	}

	float GetReliablePacketResendTime(){ return m_fReliablePacketResentTime; }

protected:

	float m_fReliablePacketResentTime;

};

#endif