#ifndef __SOUNDMANAGER_H
#define __SOUNDMANAGER_H

#include <string>
#include <map>
#include "../expat/XMLParser.h"
#include "alsound.h"


class SoundManager: public CXMLParser
{
	public:		
		SoundManager();
		~SoundManager();
			
		void load (std::string xmlSoundsFile);
		void PlaySource(const std::string& idSound);
		
		void					CleanUp		( void );
		static	SoundManager&	getInstance	( void );
		

	private:
		
		bool	mIsAvailableSound;
		std::map<std::string,Sound*> 	mSounds;
		
		void onStartElement (const std::string &elem, MKeyValue &atts);
		static SoundManager * mSoundManager;

	private:

		void _initAL		( void );
		void _finalizeAL	( void );
		
};

#endif //__SOUNDMANAGER_H
