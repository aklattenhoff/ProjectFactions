#include "Phage_Sound.h"

#include <soloud.h>
#include <soloud_wav.h>

Phage::PhageSound::PhageSound()
{
	_soundWav = NULL;
}

Phage::PhageSound::~PhageSound()
{
	delete _soundWav;
}

int Phage::PhageSound::Init(std::string filename)
{
	_soundWav = new SoLoud::Wav();
	_soundWav->load(filename.c_str());
	_soundWav->setInaudibleBehavior(false, true);

	return 0;
}

SoLoud::Wav * Phage::PhageSound::GetWav()
{
	return _soundWav;
}
