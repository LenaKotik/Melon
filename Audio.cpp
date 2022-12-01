#include "Melon.hpp"

Melon::DynamicStringArray Melon::AudioDevice::GetDeviceNames()
{
	DynamicStringArray names;
	const char* name_data = alcGetString(nullptr,ALC_DEVICE_SPECIFIER);

	do // saw this solution online, pretty clever
	{
		names.push_back(String(name_data)); // the constructor automaticaly follows the pointer until it encounters a null byte
		name_data += names.back().size() + 1; // add one to account for the null terminator
	} while (*(name_data + 1) != '\0'); // two null terminators in a row mean the end of sequence
	
	return names;
}

void Melon::AudioDevice::Delete()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(handle);
}

Melon::AudioDevice::~AudioDevice()
{
	Delete();
}

ALdouble Melon::SoundMetaData::GetDuration()
{
	return ((double)Size) / (BitsPerSample / 8.0) / Channels / SampleRate;
}

void Melon::SoundBuffer::Delete()
{
	alDeleteBuffers(1, &handle);
}