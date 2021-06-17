#include <stdio.h>
#include <string>
#include "alc.h"
#include "alut.h"
#include "defines.h"
#include "SoundManager.h"

SoundManager * SoundManager::mSoundManager=NULL;

SoundManager::SoundManager()
{
	mIsAvailableSound = true;
	_initAL();
}

SoundManager::~SoundManager()
{
	_finalizeAL();
}
void SoundManager::CleanUp( void )
{
	std::map<std::string,Sound*>::iterator it;
	for( it = mSounds.begin(); it!= mSounds.end(); ++it)
	{
		(*it).second->DestroySound();
		delete (*it).second;
	}
	CHECKED_DELETE(mSoundManager);
}

SoundManager& SoundManager::getInstance()
{
	if (mSoundManager==NULL)
	{
		mSoundManager = new SoundManager();
	}
	return *mSoundManager;
}
		

void SoundManager::PlaySource(const std::string& idSound)
{
	if( mIsAvailableSound != false )
	{
		std::map<std::string,Sound*>::iterator it;
		it = mSounds.find(idSound);
		if( it != mSounds.end() )
		{
			Sound* sound = (*it).second;
			sound->PlaySample();
		}
	}
}


void SoundManager::_initAL()
{
	ALenum error;
	ALCdevice* pDevice;
	ALCcontext* pContext;

	// Get handle to default device.
	pDevice = alcOpenDevice(NULL);

	// Get the device specifier.
	//const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

	// Create audio context.
	pContext = alcCreateContext(pDevice, NULL);

	// Set active context.
	alcMakeContextCurrent(pContext);

	// Check for an error.
	if ((error=alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		mIsAvailableSound = false;
	}
}

void SoundManager::_finalizeAL()
{
	ALCcontext* pCurContext;
	ALCdevice* pCurDevice;

	// Get the current context.
	pCurContext = alcGetCurrentContext();

	// Get the device used by that context.
	pCurDevice = alcGetContextsDevice(pCurContext);

	// Reset the current context to NULL.
	alcMakeContextCurrent(NULL);

	// Release the context and the device.
	alcDestroyContext(pCurContext);
	alcCloseDevice(pCurDevice);
}



void SoundManager::load (std::string xmlSoundsFile)
{
	// read the xml doors file
	if( mIsAvailableSound != false )
		xmlParseFile(xmlSoundsFile);
}

//parsers the xml door file
void SoundManager::onStartElement( const std::string &elem, MKeyValue &atts )
{
	if (elem=="sound2d")
	{
		if (atts["name"]!="" && atts["path"]!="")
		{
			//Process a Sound
			Sound *s = new Sound();

			std::string name  = atts["name"];
			std::string path  = atts["path"];
			
			s->LoadSound(path,false);
			mSounds.insert( std::pair<std::string,Sound*>(name,s) );		
		}
	}
}