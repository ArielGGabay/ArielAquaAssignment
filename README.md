# Overview
Hello and welcome to my Anti Virus task!
The project is divided to 3 main directories:
 - Aqua_AntiVirus - The main directory with the solution. 
 - Crypty_Virus - The directories which contains the virus.
 - TestFile - The directory that stores all the sample files.
#### Note:  The virus signature is CRYPTY_SIGNATURE.sig
 # Structure & Workflow
My anti virus searches for infected files in the given directory and in it's sub-directories using a thread pool mechanism in order to go through the files in every directory in a fast and efficient way(Implemented in ThreadPool.cpp).
In addition I have used the Boyer-Moore algorithm for searching the virus signature in the files(Implemented in SignatureSearchHandler.cpp). Eventually, I was able to activate the thread pool and execute the searching functions in FileHandler.cpp.

# Setup
1. Open CMD
2. Run Aqua_AntiVirus.exe [path_to_tests_directory] [path_to_signature]