/*
File Name: buffer.hpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- simulates a "buffer" -> holds a vector that will read elements taken from the file and perform operations on them. 
*/
#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "stream.hpp"
class buffer {
public:
	/*Constructor - sets initial value of block size*/
	buffer(size_t memoryLimit) : blockSize_(memoryLimit) {}

	void fillBuffer(std::fstream &instream);
	void sort_buffer();
	void empty_buffer(std::fstream &outstream);

private:
	std::vector<uint64_t> block;
	size_t blockSize_;
};



