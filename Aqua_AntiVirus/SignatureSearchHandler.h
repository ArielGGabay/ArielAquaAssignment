#pragma once

#include <string>
#include <map>
#include "Constants.h"

class SignatureSearchHandler
{
public :
	SignatureSearchHandler(std::string cryptySignature);
	~SignatureSearchHandler();

	bool isInfected() const;
	void createMatchingTable();
	void setFileContent(std::string fileContent);
private:

	bool charsAreEqual(int i, int shift) const;	
	
	std::string _cryptySignature;
	std::string _fileContent;
	std::map<char, int> _matchingTable;
};