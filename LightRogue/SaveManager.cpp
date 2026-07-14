#include "SaveManager.h"
#include "SaveData.h"
#include <cstdio>

const char* SaveManager::Path()
{
	return "save.dat";
}

bool SaveManager::SaveExist()
{
	FILE* f = nullptr;
	fopen_s(&f, Path(), "rb");
	if (!f)return false;
	fclose(f);
	return true;
}

bool SaveManager::Save(const SaveData& data)
{
	FILE* f = nullptr;
	fopen_s(&f, Path(), "wb");
	if (!f)return false;
	size_t written = fwrite(&data, sizeof(data), 1, f);
	fclose(f);
	return written == 1;
}

bool SaveManager::Load(SaveData& data)
{
	FILE* f = nullptr;
	fopen_s(&f, Path(), "rb");
	if (!f) return false;
	size_t read = fread(&data, sizeof(data), 1, f);
	fclose(f);
	return read == 1;
}

