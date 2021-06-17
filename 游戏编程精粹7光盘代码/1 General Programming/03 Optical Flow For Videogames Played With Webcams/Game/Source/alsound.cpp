#include <al.h>
#include <alut.h>
#include "alsound.h"

void Sound::LoadSound(std::string& inName, bool looping)
{
	//load our sound
	ALboolean loop;
	loop = looping;
	alutLoadWAVFile((ALbyte*)inName.c_str() ,&alFormatBuffer, (void **) &alBuffer,(ALsizei *)&alBufferLen, &alFreqBuffer, &loop);

	alGenSources(1, &alSource);
	alGenBuffers(1, &alSampleSet);
	alBufferData(alSampleSet, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
	alSourcei(alSource, AL_BUFFER, alSampleSet);

	alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);			

	//set the pitch
	alSourcef(alSource,AL_PITCH,1.0f);
	//set the gain
	alSourcef(alSource,AL_GAIN,1.0f);
	//set looping to true
	alSourcei(alSource,AL_LOOPING,AL_FALSE);

	alSourcei(alSource,AL_SOURCE_RELATIVE,AL_TRUE);
}



void Sound::PlaySample()
{
	alSourcePlay(alSource);
}

void Sound::StopSound()
{
	alSourceStop(alSource);
}

void Sound::DestroySound()
{
	alDeleteSources(1,&alSource);
	alDeleteBuffers(1,&alSampleSet);
}