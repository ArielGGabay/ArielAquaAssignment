#include "FileHandler.h"
#include "Constants.h"
#include <iostream>
#include <filesystem>
#include "SignatureSearchHandler.h"


//C'tor
FileHandler::FileHandler(const std::string& dirPath, ThreadPool& threadPool, const std::string& signatureFilePath)
    : _dirPath(dirPath), _threadPool(threadPool) {
    // Read the signature file content once
    _cryptySignature = readFile(signatureFilePath);
}

//D'tor
FileHandler::~FileHandler() {}

// This function iterates over all the files in a directory and its subdirectories.
// For each file encountered, it creates a task using the ThreadPool to check if the file
// is infected by the Crypty Virus. The function then waits for all tasks to complete
// before returning a vector containing the paths of all infected files.
std::vector<std::string> FileHandler::getInfectedFilesList() {
	std::vector<std::future<void>> futures;

	try {
		if (std::filesystem::exists(_dirPath) && std::filesystem::is_directory(_dirPath)) {
			// Iterate over all files in the directory and its subdirectories
			for (const auto& entry : std::filesystem::recursive_directory_iterator(_dirPath)) {
				if (!std::filesystem::is_directory(entry)) {
					// Enqueue a task for each file to check for infection using ThreadPool
					futures.push_back(_threadPool.enqueue([this, filePath = entry.path().string()]() {
						this->handleFile(filePath);
						}));
				}
			}
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
	}

	// Wait for all tasks to complete
	for (auto& future : futures) {
		future.get();
	}

	return _infectedFilesList;
}

//This function will get the current file path and first check if its an ELF file and afterwards
//it will check if the file is infected. The function will add the file to the infected files vector.
void FileHandler::handleFile(const std::string& filePath) {
	if (this->isELF(filePath)) {
		std::string fileContent = this->readFile(filePath);
		SignatureSearchHandler suspiciousFile = SignatureSearchHandler(_cryptySignature);
		suspiciousFile.setFileContent(fileContent);

		if (suspiciousFile.isInfected()) {
			std::lock_guard<std::mutex> lock(this->_infectedFilesMutex);
			this->_infectedFilesList.push_back(filePath);
		}
	}
}

//This function will get a file path and determine if it's an ELF file.
bool FileHandler::isELF(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		std::cerr << FILE_OPEN_ERROR << std::endl;
		return false;
	}

	std::vector<unsigned char> buffer(ELF_TYPE_SIGNATURE_LENGTH);
	file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

	return buffer[0] == 0x7F && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F';
}

//This function will return the size of the file.
std::streampos FileHandler::getFileSize() {
	_currentFile.seekg(0, std::ios::end);
	std::streampos size = _currentFile.tellg();
	_currentFile.seekg(0, std::ios::beg);
	return size;
}

//This function will get the path to the current binary file and read it,
//The function will return a string with the content of the file.
std::string FileHandler::readFile(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		std::cerr << FILE_OPEN_ERROR << std::endl;
		return "";
	}

	_currentFile = std::move(file); // Initialize _currentFile

	std::streampos fileSize = this->getFileSize();
	std::string fileContent(fileSize, '\0');
	_currentFile.read(&fileContent[0], fileSize);

	if (!_currentFile) {
		std::cerr << FILE_READ_ERROR << std::endl;
	}
	return fileContent;
}
