/*
File Name: stream.hpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- Stream, inputStream, and outputStream classes keep track of read/write operations
*/
#pragma once
#include <fstream>
#include<string>
#include <cstdint>
#include <sstream>
#include <boost/lexical_cast.hpp>


class stream {
public:
	stream() = default;
	std::size_t getCurrentBlockCount()const { return currentBlockCount_; }
	void decrementBlockCount() { --currentBlockCount_; }
	void resetBlockCount() { currentBlockCount_ = 0; }

protected:
	size_t currentBlockCount_ = 0;
};

class inputStream: public stream{
public:
	using size_type = std::size_t;
	inputStream() {}
	uint64_t readOneCharacter(std::fstream &instream);
	void set_number_elements_to_read(size_t size) { numberElementsRead_ = size; }
	size_type get_number_elements_to_read()const { return numberElementsRead_; }
	void set_block_count(std::size_t count) { currentBlockCount_ = count; }
	void increment_number_elements_read() { ++numberElementsRead_; }
	void decrement_number_elements_to_read() { --numberElementsRead_; }
	void reset_number_elements_read(size_t size) { numberElementsRead_ = size; }

protected: 
	size_type expectedBlockSize_ = 0;
private:
	size_type numberElementsRead_ = 0;
};

class outputStream: public stream {
public:
	outputStream() = default;
	void writeOneCharacter(std::fstream &outstream, uint64_t &number, bool flag);
	void incrementBlockCount() { ++currentBlockCount_; }
};


class fileException : public std::exception {
public:
	fileException(std::string message) : exception(message.c_str()) {};
};

template<typename STREAM_TYPE>
/*@Brief- helper function to test the health of the stream*/
inline void testStreamHealth(STREAM_TYPE &ios, std::string file, std::string operation)
{
	if (!ios)
		throw fileException("Error- unable to open " + file + " for " + operation);
}


