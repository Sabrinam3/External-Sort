/*
File Name: buffer.cpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- Buffer class method implementations
*/

#include "buffer.hpp"
#include <iostream>
using namespace std;

void buffer::fillBuffer(std::fstream &infile)
{
	auto count = 0;

	while (count != blockSize_)
	{
		if (infile.eof())
			break;
		//read in a value
		uint64_t value;
		infile >> value;
		block.push_back(value);
		++count;
	}	
}

/*@Brief- empties out the buffer by writing all the elements from the internal array to an output file*/
void buffer::empty_buffer(fstream &oss)
{
	if(oss)
	/*Writes the contents of the block to the output file*/
	oss.write((char*)block.data(), block.size() * sizeof(block[0]));
	
	else
		throw fileException("Unable to write buffer to output file");
}

/*@Brief- sorts the array of buffer elements using std sort*/
void buffer::sort_buffer()
{
	/*Sort the vector of elements*/
	if (!block.empty())
		sort(block.begin(), block.end());
}


