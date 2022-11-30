#include "Melon.hpp"

#define LOG_ERR(err) {std::cerr << "ERROR: " << #err << '\n' ;return false;}

static std::int32_t little_endian(char* num) // this whole thing is cursed
{
	// this is bellow c++20 so we use a bootleg
	std::int32_t eight = 8;
	std::int8_t* also_eight = (std::int8_t*)&eight; // we split 4-byte eight into 4 bytes
	if (*also_eight == eight) // big-endian; eight is 08 00 00 00; not 00 00 00 08
	{
		char temp[4];
		std::memcpy(temp, num, 4);
		for (int i = 0; i < 4; i++)
			num[i] = temp[3 - i]; // swap bytes
	}
	return *(std::int32_t*)num;
}
static bool LoadWavHeader(std::ifstream* file, Melon::SoundMetaData* result)
{
	char buffer[4];
	if (!file->read(buffer, 4) || buffer != "RIFF") LOG_ERR(COULD NOT READ RIFF)
	if (!file->read(buffer, 4)) LOG_ERR(COULD NOT READ LENGTH)

}
bool LoadWav(std::ifstream* file, Melon::SoundMetaData* header, char* data)
{
	if (!LoadWavHeader(file, header)) return false;
	if (!file->read(data, header->Size))LOG_ERR(COULD NOT READ WAV DATA)
	return file->fail();
}