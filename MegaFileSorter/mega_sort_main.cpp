/*
File Name: mega_sort_main.cpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- main method for the mega file sorter application
*/

#include "buffer.hpp"
#include "stream.hpp"
#include "ExternalSorter.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) try
{
	/*Provide help function if user enters inappropriate number of arguments*/
	/*No arguments*/
	if (argc == 0)
	{
		cerr << "Usage\n esort infilename outfilename [limit]" << endl;
		return EXIT_FAILURE;
	}
	else if (argc == 2)
	{
		cerr << "Error: Missing output file name\nUsage\n esort infilename outfilename [limit]" << endl;
		return EXIT_FAILURE;
	}

	size_t memoryLimit;

	/*If the user has omitted a limit, set the limit to 1000. Else, read in the limit*/
	if (argc != 4)
		 memoryLimit = 1000;
	else
	{
		string tempLimit = argv[3];
		istringstream iss(tempLimit);
		iss >> memoryLimit;
	}

	/*Calculate the block size by dividing the memory limit by the size of the container element (uint64_t)*/
	 size_t blockSize = memoryLimit / sizeof(uint64_t);

	 externalSorter sortThisFile;
	 sortThisFile.perform_sort(argv[1], argv[2], blockSize);
	
}//end main
catch (exception &e)
{
	cout << e.what() << endl;
}
catch (...)
{

}