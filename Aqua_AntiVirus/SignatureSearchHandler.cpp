#include "SignatureSearchHandler.h"
#include <iostream>
#include <iomanip>

//C'tor
SignatureSearchHandler::SignatureSearchHandler(std::string cryptySignature)
{
	this->_cryptySignature = cryptySignature;
}

//D'tor
SignatureSearchHandler::~SignatureSearchHandler()
{
}

// This function uses the Boyer-Moore algorithm to search for the signature in the
// file's content. It returns true if the file is infected, otherwise false.
bool SignatureSearchHandler::isInfected() const
{
    int shift = 0;
    int contentLength = this->_fileContent.size();
    int signatureLength = this->_cryptySignature.size();

    if (contentLength < signatureLength)
    {
        return false;
    }

    while (shift <= contentLength - signatureLength) {
        int i = signatureLength - 1;

        // Compare characters from right to left of the signature
        while (charsAreEqual(i, shift)) {
            i--;
        }

        // If all characters in the signature match, the file is infected
        if (i < 0) {
            return true;
        }
        else {
            // Use the matching table to determine the shift distance
            auto it = _matchingTable.find(this->_fileContent[i + shift]);
            if (it != _matchingTable.end()) {
                shift += it->second - i;
            }
            else {
                shift += signatureLength - i;
            }
        }
    }

    return false;
}


//This function will create a "table" with the bytes of the signature
//and the shifts of each one of them for the Boyer - Moore algorithm.
void SignatureSearchHandler::createMatchingTable() {
	int i = 0, signatureLength = this->_cryptySignature.size();

	for (i = 0; i < signatureLength; i++) {
		this->_matchingTable[this->_cryptySignature[i]] = signatureLength - i - 1;
	}

	this->_matchingTable[this->_cryptySignature[i - 1]] = signatureLength;
	this->_matchingTable[NON_EXISTING_CHARS] = signatureLength;

}

//This function will get a string with the file content and update it.
void SignatureSearchHandler::setFileContent(std::string fileContent)
{
	this->_fileContent = fileContent;
}

//This function will return whether the chars are equal or not according to their shift.
bool SignatureSearchHandler::charsAreEqual(int i, int shift) const
{
	return i >= 0 && this->_cryptySignature[i] == this->_fileContent[i + shift];
}
