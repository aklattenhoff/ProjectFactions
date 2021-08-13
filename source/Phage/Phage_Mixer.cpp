#include "Phage_Mixer.h"

#include <soloud.h>
#include <soloud_wav.h>

#include "Phage_Sound.h"

Phage::PhageMixer::PhageMixer()
{
	_soLoud = NULL;
}

Phage::PhageMixer::~PhageMixer()
{
	if (_soLoud)
		_soLoud->deinit();
	delete _soLoud;
}

int Phage::PhageMixer::Init()
{
	_soLoud = new SoLoud::Soloud();
	_soLoud->init();
	_soLoud->setMaxActiveVoiceCount(128);

	return 0;
}

int Phage::PhageMixer::SetBackgroundMusic(PhageSound* music)
{
	_currentBackgroundMusic = music;
	_soLoud->playBackground(*_currentBackgroundMusic->GetWav());

	return 0;
}

int Phage::PhageMixer::PlaySound(PhageSound* sound)
{
	if (_soLoud->getActiveVoiceCount() < _soLoud->getMaxActiveVoiceCount())
		_soLoud->play(*sound->GetWav());

	return 0;
}
