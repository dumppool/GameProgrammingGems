#include "al.h"
#include "alut.h"
#include <string>

//sound class
class Sound
{
public:
	void LoadSound(std::string& inName, bool looping);
	void PlaySample();
	void StopSound();
	void DestroySound();

private:
	char*			alBuffer;
	ALenum			alFormatBuffer;
	ALsizei			alFreqBuffer;
	long			alBufferLen;
	ALboolean		alLoop;
	unsigned int	alSource;
	unsigned int	alSampleSet;
};