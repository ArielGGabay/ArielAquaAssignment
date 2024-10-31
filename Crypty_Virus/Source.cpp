#include "FileInfector.h"
#include <iostream>

int main()
{
	
	FileInfector fileInfector = FileInfector();
	std::cout << "Enter path to the file you want to infect:" << std::endl;
	fileInfector.infectFile("PATH TO FILE");	

	return 0;
}