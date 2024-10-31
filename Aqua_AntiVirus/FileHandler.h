#pragma once
#include "ThreadPool.h"
#include "SignatureSearchHandler.h"

#include <string>
#include <vector>
#include <fstream>
#include <future>


class FileHandler {
public:
    FileHandler(const std::string& dirPath, ThreadPool& threadPool, const std::string& signatureFilePath);

    ~FileHandler();

    std::string readFile(const std::string& filePath);
    std::vector<std::string> getInfectedFilesList();

    void handleFile(const std::string& filePath);

private:
    bool isELF(const std::string& filePath);
    std::streampos getFileSize();

    std::string _dirPath;
    std::string _cryptySignature;
    std::string _signatureFilePath;

    ThreadPool& _threadPool;

    std::vector<std::string> _infectedFilesList;
    std::mutex _infectedFilesMutex;
    std::ifstream _currentFile;
};
