#include "Melon.hpp"

#define LOG_ERR(err) {std::cerr << "ERROR: " << #err << '\n' ;return false;}

static std::int32_t big_endian(char* dword) // this whole thing is cursed
{
	// this is bellow c++20 so we use a bootleg
	std::int32_t eight = 8;
	std::int8_t* also_eight = (std::int8_t*)&eight; // we split 4-byte eight into 4 bytes
	if (!*also_eight) // little-endian; eight is 00 00 00 08; not 08 00 00 00; that means first byte is 0
	{
		char temp[4];
		std::memcpy(temp, dword, 4);
		for (int i = 0; i < 4; i++)
			dword[i] = temp[3 - i]; // swap bytes
	}
	return *(std::int32_t*)dword;
}
bool LoadWavHeader(std::ifstream* file, Melon::SoundMetaData* result)
{
	char buffer[5];
	buffer[4] = '\0'; // set last to null, we never write here anyway
	if (!file->read(buffer, 4) || std::strcmp(buffer,"RIFF")) LOG_ERR(COULD NOT READ RIFF)

	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ LENGTH)

	if (!file->read(buffer, 4) || std::strcmp(buffer, "WAVE")) LOG_ERR(COULD NOT READ WAVE)

	if (!file->read(buffer, 4) || std::strcmp(buffer, "fmt ")) LOG_ERR(COULD NOT READ FMT SUBCHUNK)

	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ FMT SIZE)
	if (big_endian(buffer) != 0x10) LOG_ERR(INVALID FMT SIZE)

	if (!file->read(buffer, 2)) LOG_ERR(COULD NOT READ AUDIO FORMAT)
	if (*buffer != 1) LOG_ERR(COMPRESSION NOT SUPPORTED)

	if (!file->read(buffer, 2)) LOG_ERR(COULD NOT READ N CHANNELS)
	result->Channels = *buffer; // one byte should be enough?

	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ SAMPLE RATE)
	result->SampleRate = big_endian(buffer);

	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ BYTE RATE)

	if (!file->read(buffer, 2)) LOG_ERR(COULD NOT READ BLOCK ALIGN)

	if (!file->read(buffer, 2)) LOG_ERR(COULD NOT READ DEPTH)
	result->BitsPerSample = *buffer; // one byte should be enough?

	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ SECOND CHUNK)
	if (std::strcmp(buffer, "data")) // we encountered an odd chunk, so we skip it
	{
		if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ SECOND CHUNK SIZE)
		file->seekg(big_endian(buffer), SEEK_CUR);
		if (!file->read(buffer, 4) || std::strcmp(buffer, "data")) LOG_ERR(COULD NOT READ DATA CHUNK)
	}
	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ DATA SIZE)
	result->Size = big_endian(buffer);
	
	return !file->fail();
}
bool Melon::LoadWav_(std::ifstream* file, Melon::SoundMetaData* header, char* data)
{
	if (!LoadWavHeader(file, header)) return false;
	data = new char[header->Size];
	if (!file->read(data, header->Size))LOG_ERR(COULD NOT READ WAV DATA)
	return !file->fail();
}