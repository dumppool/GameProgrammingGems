#ifndef _NETWORK_TWEAKABLES_H_
#define _NETWORK_TWEAKABLES_H_

class NetworkTweakables
{
public:
	NetworkTweakables():
	m_fReliablePacketResentTime(2.0f),
	m_fBrowseTimeout(2.0f)
	{

	}

	static NetworkTweakables& GetInstance()
	{
		static NetworkTweakables sNetworkTweakables;
		return sNetworkTweakables;
	}

	float GetReliablePacketResendTime(){ return m_fReliablePacketResentTime; }
	float GetBrowseTimeout(){ return m_fBrowseTimeout; }

protected:

	float m_fReliablePacketResentTime;
	float m_fBrowseTimeout;

};

#endif