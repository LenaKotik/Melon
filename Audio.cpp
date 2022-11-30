#include "Melon.hpp"

ALdouble Melon::SoundMetaData::GetDuration()
{
	return ((double)Size) / (BitsPerSample / 8.0) / Channels / SampleRate;
}