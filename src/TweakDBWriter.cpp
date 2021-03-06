#include <iostream>
#include <fstream>
#include <RED4ext/Hashing/CRC.hpp>
#include "TweakDBWriter.hpp"
#include <filesystem>

unsigned char part_one[20] = { 0x47, 0xDB, 0xB1, 0x0B, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00 };
uint32_t offsets[3]  = { 0x01A0, 0x01A4, 0x01A8 };
unsigned char part_two[20] = { 0x01, 0x00, 0x00, 0x00, 0x2F, 0x34, 0x21, 0x85, 0x74, 0xDE, 0x57, 0x2E, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
uint32_t count       = 0x2B;
unsigned char part_thr[344] = {
	0x0D, 0x2E, 0x32, 0xE3, 0x26, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x51, 0x20, 0x24, 0x00, 0x00, 0x00, 0x05, 0x5C, 0xF0, 0xC0, 0x26, 0x00, 0x00, 0x00, 0x76, 0x78, 0x5C, 0x4A, 0x2B, 0x00, 0x00, 0x00, 0xFA, 0x6B, 0x29, 0x99, 0x29, 0x00, 0x00, 0x00,
	0xC7, 0x8A, 0x7E, 0x7A, 0x26, 0x00, 0x00, 0x00, 0xB5, 0xC6, 0x0B, 0xAE, 0x2C, 0x00, 0x00, 0x00, 0x23, 0x98, 0x95, 0x3C, 0x28, 0x00, 0x00, 0x00, 0x5C, 0xB1, 0xB6, 0xAA, 0x25, 0x00, 0x00, 0x00, 0x8A, 0xD6, 0xE4, 0x38, 0x2D, 0x00, 0x00, 0x00,
	0x7B, 0x5A, 0x2B, 0x31, 0x2B, 0x00, 0x00, 0x00, 0x43, 0x9B, 0x61, 0x51, 0x2A, 0x00, 0x00, 0x00, 0x5E, 0xFE, 0x75, 0x67, 0x28, 0x00, 0x00, 0x00, 0xF0, 0x94, 0xFF, 0xA5, 0x2F, 0x00, 0x00, 0x00, 0x7A, 0xEC, 0x4D, 0xED, 0x2E, 0x00, 0x00, 0x00,
	0x4E, 0x8E, 0x17, 0x9E, 0x30, 0x00, 0x00, 0x00, 0xB0, 0xDE, 0x02, 0x70, 0x27, 0x00, 0x00, 0x00, 0x87, 0x76, 0xC3, 0x0D, 0x28, 0x00, 0x00, 0x00, 0xA4, 0xBF, 0x5C, 0x33, 0x2A, 0x00, 0x00, 0x00, 0x64, 0xDC, 0x02, 0x97, 0x29, 0x00, 0x00, 0x00,
	0x7F, 0x5C, 0x5F, 0xC7, 0x28, 0x00, 0x00, 0x00, 0xBC, 0xEE, 0xA5, 0x0F, 0x29, 0x00, 0x00, 0x00, 0x19, 0x7E, 0x33, 0x00, 0x2F, 0x00, 0x00, 0x00, 0xDF, 0xBE, 0xAC, 0xE7, 0x2B, 0x00, 0x00, 0x00, 0xA3, 0x2E, 0x4C, 0xF7, 0x34, 0x00, 0x00, 0x00,
	0xF1, 0x05, 0x80, 0x2A, 0x2C, 0x00, 0x00, 0x00, 0xB2, 0x05, 0x6F, 0xAA, 0x2B, 0x00, 0x00, 0x00, 0x46, 0x76, 0x11, 0x85, 0x2A, 0x00, 0x00, 0x00, 0x47, 0x05, 0x14, 0x39, 0x20, 0x00, 0x00, 0x00, 0x29, 0x5E, 0xB0, 0xC6, 0x2A, 0x00, 0x00, 0x00,
	0x9F, 0xFB, 0xE6, 0xA4, 0x30, 0x00, 0x00, 0x00, 0xEB, 0x23, 0x82, 0x53, 0x27, 0x00, 0x00, 0x00, 0xB2, 0x59, 0xD3, 0x44, 0x2D, 0x00, 0x00, 0x00, 0x1C, 0xED, 0xC5, 0x60, 0x35, 0x00, 0x00, 0x00, 0x27, 0x50, 0x4C, 0x82, 0x2F, 0x00, 0x00, 0x00,
	0x05, 0x1D, 0x20, 0x04, 0x2F, 0x00, 0x00, 0x00, 0xC9, 0x15, 0x4D, 0x9B, 0x2F, 0x00, 0x00, 0x00, 0x50, 0x9D, 0x88, 0x7B, 0x30, 0x00, 0x00, 0x00, 0xAC, 0x23, 0x3C, 0xF5, 0x26, 0x00, 0x00, 0x00, 0x1E, 0x67, 0xD6, 0xC5, 0x2F, 0x00, 0x00, 0x00,
	0x94, 0x2E, 0xD9, 0x70, 0x2C, 0x00, 0x00, 0x00, 0x5D, 0x62, 0xE6, 0x50, 0x33, 0x00, 0x00, 0x00, 0x6D, 0xD8, 0x5D, 0xFA, 0x28, 0x00, 0x00, 0x00
};
unsigned char part_fou[28] = {
	0x01, 0x00, 0x00, 0x00, 0x21, 0x44, 0xA7, 0xB4, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
struct TweakDBID
{
#pragma pack(push, 1)
	union
	{
		uint64_t value = 0;
		struct
		{
			uint32_t nameHash;      // 00 CRC32
			uint8_t nameLength;     // 04
			uint8_t tdbOffsetBE[3]; // 05 - Big Endian
		};
	};
#pragma pack(pop)
};

void writeTweakDB(std::vector<std::string> vehicles, std::filesystem::path path) {
	uint32_t vehicleCount = vehicles.size();

	std::ofstream f;
	f.open(path, std::ofstream::binary);

	f.write((char*)&part_one, sizeof(part_one));

	offsets[0] += (vehicleCount * 8);
	offsets[1] += (vehicleCount * 8);
	offsets[2] += (vehicleCount * 8);
	f.write((char*)&offsets[0], sizeof(offsets[0]));
	f.write((char*)&offsets[1], sizeof(offsets[1]));
	f.write((char*)&offsets[2], sizeof(offsets[2]));

	f.write((char*)&part_two, sizeof(part_two));

	count += vehicleCount;
	f.write((char*)&count, sizeof(count));

	f.write((char*)&part_thr, sizeof(part_thr));

	for (std::string vehicle : vehicles) {
		size_t len = vehicle.size();
		TweakDBID twdb = TweakDBID();
		twdb.nameLength = static_cast<uint8_t>(len);
		twdb.nameHash = RED4ext::CRC32(reinterpret_cast<const uint8_t*>(vehicle.data()), len, 0);
		std::memset(twdb.tdbOffsetBE, 0, sizeof(twdb.tdbOffsetBE));
		f.write((char*)&twdb, sizeof(twdb));
	}

	f.write((char*)&part_fou, sizeof(part_fou));
	f.close();
}