
#include "FileHandler.h"
#include "Constants.h"
#include "ThreadPool.h"
#include <iostream>

int main(int arc, char* argv[]) {

	if (arc < 2)
	{
		std::cout << "You must input the path of the directory and the signature file" << std::endl;
		return 0;
	}
	ThreadPool threadPool(MAX_SIMULTANEOUS_THREADS);

	std::string directoryPath = argv[DIRECTORY_PATH_INDEX];
	FileHandler fileHandler(directoryPath, threadPool, argv[SIGNATURE_FILE_PATH_INDEX]);

	std::vector<std::string> infectedFiles = fileHandler.getInfectedFilesList();

	//Printing the list of all the infected files.
	std::cout << "Infected files:" << std::endl;
	for (const std::string& file : infectedFiles) {
		std::cout << file << std::endl;
	}

	return 0;
}
