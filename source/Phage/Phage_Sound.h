#ifndef _PHAGE_SOUND_H
#define _PHAGE_SOUND_H

#include <stdlib.h>
#include <string>

namespace SoLoud
{
	class Wav;
}

namespace Phage
{
	class PhageSound
	{
	public:
		PhageSound();
		~PhageSound();

		int Init(std::string filename);
		SoLoud::Wav* GetWav();
	private:
		SoLoud::Wav* _soundWav;
	};
}

#endif
