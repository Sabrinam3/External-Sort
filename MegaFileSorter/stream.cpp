/*
File Name: stream.cpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- input/output stream class method implementations
*/

#include "stream.hpp"
using namespace std;


/*@Brief- reads one character from an input file source and converts from binary to uint64_t*/
uint64_t inputStream::readOneCharacter(fstream &instream)
{
	if (instream.good())
	{
		uint64_t value;
		instream.read((char*)&value, sizeof(value));
		return value;
	}
}

/*@Brief- writes one character to an output file source either in binary or regular format depending on 'flag'*/
void outputStream::writeOneCharacter(fstream &outstream, uint64_t &number, bool flag)
{
	if (outstream)
	{
		if (flag)
		{
			/*Write the value to the output stream in binary*/
			outstream.write((char*)&number, sizeof(number));
		}
		else
		{
			/*Cast to a string before writing to file*/
			string val = boost::lexical_cast<std::string>(number);
			outstream << val << "\n";
		}
	
	}
		
	else
		throw fileException("Error writing character to stream");
}


