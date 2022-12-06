#include "Melon.hpp"

Melon::DynamicStringArray Melon::AudioDevice::GetDeviceNames()
{
	DynamicStringArray names;
	const char* name_data = alcGetString(nullptr,ALC_DEVICE_SPECIFIER);

	do // saw this solution online, pretty clever
	{
		names.PushBack(String(name_data)); // the constructor automaticaly follows the pointer until it encounters a null byte
		name_data += names.PeekBack().size()+ 1; // add one to account for the null terminator
	} while (*(name_data + 1) != '\0'); // two null terminators in a row mean the end of sequence
	
	return names;
}

Melon::String Melon::AudioDevice::GetName()
{
	return alcGetString(handle, ALC_DEVICE_SPECIFIER);
}

ALenum Melon::AudioDevice::GetDeviceError()
{
	return alcGetError(handle);
}

ALenum Melon::AudioDevice::GetContextError()
{
	return alGetError();
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

ALdouble Melon::AudioHeaderData::GetDuration()
{
	return ((double)Size) / (BitsPerSample / 8.0) / Channels / SampleRate;
}

void Melon::AudioBuffer::Delete()
{
	alDeleteBuffers(1, &handle);
}

Melon::AudioSource::AudioSource() : 
	Pitch(1), Gain(1), Position(0), Velocity(0), Loop(0), StreamingBufferCount(4), StreamingBufferSize(1024)
{
	alGenSources(1, &handle);
}

void Melon::AudioSource::Play(AudioBuffer* buffer)
{
	alSourcef(handle, AL_PITCH, Pitch);
	alSourcef(handle, AL_GAIN, Gain);
	alSource3f(handle, AL_POSITION, Position.x, Position.y, Position.z);
	alSource3f(handle, AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
	alSourcei(handle, AL_LOOPING, Loop);
	alSourcei(handle, AL_BUFFER, buffer->handle);

	alSourcePlay(handle);
}

void Melon::AudioSource::Play(InputStream* stream, AudioHeaderData* header)
{
}

void Melon::AudioSource::Resume()
{
	alSourcePlay(handle);
}

void Melon::AudioSource::Stop()
{
	alSourceStop(handle);
}

void Melon::AudioSource::Pause()
{
	alSourcePause(handle);
}

void Melon::AudioSource::Rewind()
{
	alSourceRewind(handle);
}

bool Melon::AudioSource::IsPlaying()
{
	ALint state;
	alGetSourcei(handle, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Melon::AudioSource::UpdateStreaming(InputStream* same_stream)
{
}

void Melon::AudioSource::Delete()
{
	alDeleteSources(1, &handle);
}
