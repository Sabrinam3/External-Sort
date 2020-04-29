
/*
File Name: ExternalSorter.cpp
Author: Sabrina Tessier
Date: Dec 5, 2018
@Brief- external sorter class method implementations
*/

#include "ExternalSorter.hpp"
using namespace std;
using namespace experimental::filesystem;
using namespace boost::filesystem;


/*Construct static objects*/
inputStream externalSorter::istream1;
inputStream externalSorter::istream2;
outputStream externalSorter::ostream1;
outputStream externalSorter::ostream2;

/*@Brief- creates a directory in the user's current working directory to host the temp files that this program will create*/
void externalSorter::create_temp_directory() {
	boost::filesystem::path curr_path = boost::filesystem::current_path();
	boost::filesystem::path rel_path = "/tempfiles";
	boost::filesystem::path combined = (curr_path /= rel_path);
	boost::filesystem::create_directories(combined);
}

/*@Brief- sets the current mode and source of the input file sources*/
void externalSorter::set_input_file_source(std::fstream &iss1, std::fstream &iss2, bool readStatus, size_t &blockSize)
{
	/*Close existing connections*/
	iss1.close();
	iss2.close();

	if (readStatus)
	{
		iss1.open("tempfiles/temp1.txt", ios::in | ios::binary);
		iss2.open("tempfiles/temp2.txt", ios::in | ios::binary);
		testStreamHealth(iss1, "tempfiles/temp1.txt", "reading");
		testStreamHealth(iss2, "tempfiles/temp2.txt", "reading");
	}
	else
	{
		iss1.open("tempfiles/temp3.txt", ios::in | ios::binary);
		iss2.open("tempfiles/temp4.txt", ios::in | ios::binary);
		testStreamHealth(iss1, "tempfiles/temp3.txt", "reading");
		testStreamHealth(iss2, "tempfiles/temp4.txt", "reading");
	}



	/*Set the input stream object block counts to be the same as what the output block count was and then reset the output block count*/
	istream1.set_block_count(ostream1.getCurrentBlockCount());
	istream2.set_block_count(ostream2.getCurrentBlockCount());
	/*Set the number of values to read for each file to be equal to the current block size*/
	istream1.set_number_elements_to_read(blockSize);
	istream2.set_number_elements_to_read(blockSize);
	ostream1.resetBlockCount();
	ostream2.resetBlockCount();
}

/*@Brief- sets the current mode and source of the output file sources*/
void externalSorter::set_output_file_source(std::fstream &oss1, std::fstream &oss2, bool readStatus) 
{
	/*Close fstream's from last read operation*/
	oss1.close();
	oss2.close();
	if (readStatus)
	{
		/*Open the file with truncate on to clear current contents*/
		oss1.open("tempfiles/temp3.txt", ios::out | ios::trunc | ios::binary);
		oss2.open("tempfiles/temp4.txt", ios::out | ios::trunc | ios::binary);
		/*Close the files and reopen in append mode*/
		oss1.close();
		oss1.open("tempfiles/temp3.txt", ios::out | ios::app | ios::binary);
		oss2.close();
		oss2.open("tempfiles/temp4.txt", ios::out | ios::app | ios::binary);
		testStreamHealth(oss1, "tempfiles/temp3.txt", "writing");
		testStreamHealth(oss2, "tempfiles/temp4.txt", "writing");
	}
	else
	{
		/*Open the file with truncate on to clear current contents*/
		oss1.open("tempfiles/temp1.txt", ios::out | ios::trunc | ios::binary);
		oss2.open("tempfiles/temp2.txt", ios::out | ios::trunc | ios::binary);
		/*Close the files and reopen in append mode*/
		oss1.close();
		oss1.open("tempfiles/temp1.txt", ios::out | ios::app | ios::binary);
		oss2.close();
		oss2.open("tempfiles/temp2.txt", ios::out | ios::app | ios::binary);
		testStreamHealth(oss1, "tempfiles/temp1.txt", "writing");
		testStreamHealth(oss2, "tempfiles/temp2.txt", "writing");
	}

}

/*@Brief- counts the number of integers in a binary file*/
size_t externalSorter::count_integers_in_file(std::fstream &iss)
{
	iss.seekg(0, ios::end);
	size_t lengthInBytes = iss.tellg();
	iss.seekg(0, ios::beg);
	return lengthInBytes / 8;
}

void externalSorter::perform_initial_read_write(std::fstream &iss, char * source, size_t blockSize)
{
	/*Create an input file stream to read from the initial file, open it, and check that the operation was successful*/
	iss.open(source, ios::in);
	testStreamHealth(iss, source, "reading");

	/*Create two out files which will allow the blocks of data to be written to and ensure the opening was successful*/
	std::fstream oss1("tempfiles/temp1.txt", ios::app | ios::binary | ios::out);
	testStreamHealth(oss1, "temp1.txt", "writing");
	std::fstream oss2("tempfiles/temp2.txt", ios::app | ios::binary | ios::out);
	testStreamHealth(oss2, "temp2.txt", "writing");

	/*Bool to control the current stream. True will write to output 1, false to output 2*/
	bool currentStream = true;

	/*Read from the initial file until eof is reached*/
	while (!iss.eof())
	{
		/*Create a buffer object and fill it*/
		buffer b(blockSize);
		b.fillBuffer(iss);
		b.sort_buffer();
		
		/*If the current stream is true, write to the first output file. Else write to the second*/
		currentStream ? b.empty_buffer(oss1) : b.empty_buffer(oss2);

		/*Increment the block count of the current stream*/
		currentStream ? ostream1.incrementBlockCount() : ostream2.incrementBlockCount();

		/*Flip the boolean*/
		if (currentStream) currentStream = false;
		else currentStream = true;
		
	}

	/*Close the input and output files*/
	iss.close();
	oss1.close();
	oss2.close();
}//End initial read write

/*@Brief, reads in two values from a binary file, determines which one is smaller and returns that one. Returns the smaller one to the stream*/
uint64_t externalSorter::read_smaller_value(std::fstream &iss1, std::fstream &iss2)
{
	auto val1 = istream1.readOneCharacter(iss1);
	auto val2 = istream2.readOneCharacter(iss2);
	if (iss1.eof())return val2;
	if (iss2.eof())return val1;

	if (val1 < val2)
	{
		istream1.decrement_number_elements_to_read();
		iss2.seekg(-8, ios::cur);
		return val1;
	}
	else
	{
		istream2.decrement_number_elements_to_read();
		iss1.seekg(-8, ios::cur);
		return val2;
	}

}
/*@Brief- deletes the temporary directory that was created*/
void externalSorter::delete_temp_directory()
{
	boost::filesystem::path curr_path = boost::filesystem::current_path();
	boost::filesystem::path rel_path = "/tempfiles";
	boost::filesystem::path combined = (curr_path /= rel_path);
	boost::filesystem::remove_all(combined);
}

/*@Brief- writes remaining values from an input stream to the output stream until the number of elements to read of the input stream reaches 0,
then flips the bool so that the streams will be switched*/
void externalSorter::write_remaining_block(std::fstream &oss1, std::fstream &oss2, std::fstream &iss, inputStream &istream, bool &currentStream)
{
	while (istream.get_number_elements_to_read() > 0)
	{
		auto val = istream.readOneCharacter(iss);
		currentStream ? ostream1.writeOneCharacter(oss1, val, true) : ostream2.writeOneCharacter(oss2, val, true);
		istream.decrement_number_elements_to_read();
	}

	/*Increment the block count for both of the output streams*/
	currentStream ? ostream1.incrementBlockCount() : ostream2.incrementBlockCount();

	/*Flip the current stream*/
	if (currentStream)
		currentStream = false;
	else
		currentStream = true;
}

/*@Brief returns true if there is at least one integer left in the input stream*/
bool externalSorter::check_remaining_elements(std::fstream &iss, inputStream &inputStream)
{
	auto val = inputStream.readOneCharacter(iss);
	if (!iss.eof())
	{
		iss.seekg(-8, ios::cur);
		return true;	
	}
	else
		return false;
}

/*@Brief- writes remaining elements from a file to the current output stream*/
void externalSorter::write_remaining_elements(inputStream &istream, std::fstream &oss1, std::fstream &oss2, std::fstream &iss, bool &currentStream)
{
	while (true)
	{
		auto val = istream.readOneCharacter(iss);
		if (!iss.eof())
		{
			currentStream ? ostream1.writeOneCharacter(oss1, val, true) : ostream2.writeOneCharacter(oss2, val, true);
		}
		/*Increment the block count for the current output stream and break*/
		else
		{
			currentStream ? ostream1.incrementBlockCount() : ostream2.incrementBlockCount();
			break;
		}
	}
}


/*@Brief- performs the final write operation from the two input files to the user's specified output file*/
void externalSorter::perform_final_write(std::fstream &iss1, std::fstream &iss2, char * outputSource)
{
	/*Create a final output stream*/
	std::fstream finalOutputStream(outputSource, ios::app | ios::out);
	outputStream finalStream;
	while (true)
	{
		auto val = read_smaller_value(iss1, iss2);
		if (iss1.eof())
		{
			finalStream.writeOneCharacter(finalOutputStream, val, false);
			break;
		}
		else if (iss2.eof())
		{
			finalStream.writeOneCharacter(finalOutputStream, val, false);
			break;
		}

		finalStream.writeOneCharacter(finalOutputStream, val, false);
	}
	/*Figure out which file still has values*/
	if (check_remaining_elements(iss1, istream1))
	{
		while (true)
		{
			auto val = istream1.readOneCharacter(iss1);
			if (!iss1.eof())
				finalStream.writeOneCharacter(finalOutputStream, val, false);
			else
				break;
		}
	}
	else if (check_remaining_elements(iss2, istream2))
	{
		while (true)
		{
			auto val = istream2.readOneCharacter(iss2);
			if (!iss2.eof())
				finalStream.writeOneCharacter(finalOutputStream, val, false);
			else
				break;
		}
	}

	/*Close the last remaining input/output files*/
	finalOutputStream.close();
	iss1.close();
	iss2.close();
}

/*@Brief- main sort method that will be called from mega_sort_main*/
void externalSorter::perform_sort(char * inputSource, char * outputSource, std::size_t blockSize)
{
	/*Create a directory to host temporary files*/
	create_temp_directory();

	/*Bool to control streams- true means write to stream1, false to stream2*/
	bool currentStream = true;
	/*Controls the flipping of the temp files between read/write*/
	bool readStatus = true;

	/*Declare fstream objects*/
	std::fstream initialFileReader;
	perform_initial_read_write(initialFileReader, inputSource, blockSize);
	
	std::fstream fs1; //currently set to temp1
	std::fstream fs2; //temp2
	std::fstream fs3; //temp3
	std::fstream fs4; //temp4

	/*Set the previous write files to be the read files*/
	set_input_file_source(fs1, fs2, readStatus, blockSize);
	set_output_file_source(fs3, fs4, readStatus);


	/*Loop while there is more than one block in each file*/
	while (istream1.getCurrentBlockCount() > 1 || istream2.getCurrentBlockCount() > 1)
	{
		/*Determine the number of integers in each file*/
		size_t length1 = count_integers_in_file(fs1);
		size_t length2 = count_integers_in_file(fs2);
		/*Determine the number of full block pairs in each file*/
		size_t blocks1 = length1 / blockSize;
		size_t blocks2 = length2 / blockSize;
		/*Set the number of full block pairs to be the minimum of the blocks in each file*/
		size_t fullBlockPairs = min(blocks1, blocks2);
		/*Iterate through the full number of block pairs*/
		for (auto i = 0; i < fullBlockPairs; ++i)
		{
			/*Iterate while there are more than 0 items to read in either of the blocks in the current pair*/
			while (istream1.get_number_elements_to_read() > 0 && istream2.get_number_elements_to_read() > 0)
			{
				auto val = read_smaller_value(fs1, fs2);
				/*Write the smaller value to the current output stream*/
				currentStream ? ostream1.writeOneCharacter(fs3, val, true) : ostream2.writeOneCharacter(fs4, val, true);
			}//One of the blocks in the block pairs has been completely read

			/*Figure out which block has remaining elements and write them to the current output stream*/
			if (istream1.get_number_elements_to_read() > 0)
			{
				write_remaining_block(fs3, fs4, fs1, istream1, currentStream);
			}
			else if (istream2.get_number_elements_to_read() > 0)
			{
				write_remaining_block(fs3, fs4, fs2, istream2, currentStream);
			}

			/*Reset the number of elements to read for each input stream*/
			istream1.reset_number_elements_read(blockSize);
			istream2.reset_number_elements_read(blockSize);
		}//All full block pairs have been processed

		/*Figure out which input file has any elements left and write the remaining elements to the current output stream*/
		/*Elements remaining in first stream but not second*/
		if (check_remaining_elements(fs1, istream1) && !check_remaining_elements(fs2, istream2))
		{
			write_remaining_elements(istream1, fs3, fs4, fs1, currentStream);
		}
		/*Elements remaining in second stream but not first*/
		else if (!check_remaining_elements(fs1, istream1) && check_remaining_elements(fs2, istream2))
		{
			write_remaining_elements(istream2, fs3, fs4, fs2, currentStream);
		}
		//There are some elements left in both
		else if (check_remaining_elements(fs1, istream1) && check_remaining_elements(fs2, istream2))
		{
			while (true)
			{
				auto val = read_smaller_value(fs1, fs2);
				if (fs1.eof() || fs2.eof())
				{
					currentStream ? ostream1.writeOneCharacter(fs3, val, true) : ostream2.writeOneCharacter(fs4, val, true);
					break;
				}
				currentStream ? ostream1.writeOneCharacter(fs3, val, true) : ostream2.writeOneCharacter(fs4, val, true);
			}

			if (fs1.good())
			{
				while (true)
				{
					auto val = istream1.readOneCharacter(fs1);
					if (fs1.eof())break;
					currentStream ? ostream1.writeOneCharacter(fs3, val, true) : ostream2.writeOneCharacter(fs4, val, true);
				}
			}
			else if (fs2.good())
			{
				while (true)
				{
					auto val = istream2.readOneCharacter(fs2);
					if (fs2.eof())break;
					currentStream ? ostream1.writeOneCharacter(fs3, val, true) : ostream2.writeOneCharacter(fs4, val, true);
				}
			}

			currentStream ? ostream1.incrementBlockCount() : ostream2.incrementBlockCount();
		}

		/*Set blocksize to twice what it was before*/
		blockSize *= 2;

		/*Flip read status*/
		if (readStatus) readStatus = false;
		else readStatus = true;
		/*Make the current output steams the new input streams*/
		set_input_file_source(fs1, fs2, readStatus, blockSize);
		set_output_file_source(fs3, fs4, readStatus);	
	}//block count of 1 has been reached on both files

	/*Final step - write the results of the current input files to the final outpu file specified by the user*/
	perform_final_write(fs1, fs2, outputSource);

	/*Close the streams*/
	fs1.close();
	fs2.close();
	fs3.close();
	fs4.close();
	/*Delete the temporary directory*/
	delete_temp_directory();
	
}
