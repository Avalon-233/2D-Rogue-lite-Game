#pragma once

//designed with AI assistance

struct SaveData;

class SaveManager
{
public:
	static bool SaveExist();
	static bool Save(const SaveData& data);
	static bool Load(SaveData& data);

private:
	static const char* Path();
};