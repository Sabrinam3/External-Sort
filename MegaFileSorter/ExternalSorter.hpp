/*
File Name: ExternalSorter.hpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- external sorter object performs the sorting operations by reading/writing the files
*/

#pragma once
#include "buffer.hpp"
#include "stream.hpp"
#include <filesystem>
#include <boost/filesystem.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <fstream>

class externalSorter{

public:
	void perform_sort(char * inputSource, char * outputSource, std::size_t blockSize);
	
	/*Static member functions*/
	static size_t count_integers_in_file(std::fstream &iss);
	static void perform_initial_read_write(std::fstream &iss, char * source, size_t blockSize);
	static void create_temp_directory();
	static void set_input_file_source(std::fstream &iss1, std::fstream &iss2, bool readStatus, size_t &blockSize);
	static void set_output_file_source(std::fstream &oss1, std::fstream &oss2, bool readStatus);
	static void delete_temp_directory();
	static uint64_t read_smaller_value(std::fstream &iss1, std::fstream &iss2);
	static void write_remaining_block(std::fstream &oss1, std::fstream &oss2, std::fstream &iss, inputStream &istream, bool &currentStream);
	static bool check_remaining_elements(std::fstream &iss, inputStream &inputStream);
	static void write_remaining_elements(inputStream &istream, std::fstream &oss1, std::fstream &oss2, std::fstream &iss, bool &currentStream);
	static void perform_final_write(std::fstream &iss1, std::fstream &iss2, char * outputSource);
	/*Static member variables*/
	static outputStream ostream1;
	static outputStream ostream2;
	static inputStream istream1;
	static inputStream istream2;

};

