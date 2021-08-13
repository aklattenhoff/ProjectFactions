#ifndef _PHAGE_MIXER_H
#define _PHAGE_MIXER_H

#include <stdlib.h>
#include <string>

namespace SoLoud
{
	class Soloud;
	class Wav;
}

namespace Phage
{
	class PhageSound;
	class PhageMixer
	{
	public:
		PhageMixer();
		~PhageMixer();

		int Init();
		int SetBackgroundMusic(PhageSound* music);
		int PlaySound(PhageSound* sound);
	private:
		SoLoud::Soloud* _soLoud;
		PhageSound* _currentBackgroundMusic;
	};
}

#endif
