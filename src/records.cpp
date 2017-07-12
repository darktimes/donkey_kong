#include <fstream>
#include "records.hpp"
#include "utils.hpp"

using namespace Engine;
using namespace Utils;

std::string const * const RecordTable::RECORD_TABLE_FILE = new std::string("records");

const unsigned int RecordTable::RECORD_COUNT = 5;

Record::Record(int score, std::string playerName) : score(score), playerName(playerName) {

}

RecordTable::RecordTable() {

}

RecordTable::~RecordTable() {
	for (std::vector<Record*>::iterator it = records.begin() ; it != records.end(); ++it) {
		delete *it;
	}
}

void RecordTable::readTable() {
	std::string file_path = *retreive_binary_path();
	file_path += *RecordTable::RECORD_TABLE_FILE;
	std::string result;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		fileStream.open(file_path.c_str());
		std::getline(fileStream, result);
	}
	catch (std::ifstream::failure e) {
		Logger::w("RecordTable will be init-ed with default due to stream-error.");
		initDefaults();
	}


	if (!parseStringAsRecords(result)) {
		Logger::w("RecordTable will be init-ed with default due to parse-error.");
		initDefaults();
	} else {
		Logger::i("RecordTable succesfully loaded");
	}
}

void RecordTable::saveTable() {
	remove(RecordTable::RECORD_TABLE_FILE->c_str());
	std::ofstream outputTableStream;
	outputTableStream.open(RecordTable::RECORD_TABLE_FILE->c_str());
	for (auto const& record: records) {
		outputTableStream<<record->score<<","<<record->playerName<<",";
	}
	outputTableStream.close();
}

bool RecordTable::parseStringAsRecords(std::string& str) {
	std::string delim(",");
	int start = 0;
	size_t end = str.find(delim);
	int currentScore;
	unsigned i = 0;

	while (end != std::string::npos && i < (RecordTable::RECORD_COUNT * 2)) {
		std::string current_block = str.substr(start, end - start);
		start = end + delim.length();
	        end = str.find(delim, start);
		if ( (i % 2) == 0) {
			try {
				currentScore = std::stoi(current_block);
			} catch (...) {
				return false;
			}
		} else {
			records.push_back(new Record(currentScore, current_block));
		}
		i++;
	}
	if (records.size() != RecordTable::RECORD_COUNT) {
		return false;
	}
	return true;
}

void RecordTable::initDefaults() {
	records.clear();
	records.push_back(new Record(3000, "Franklin"));
	records.push_back(new Record(2500, "George"));
	records.push_back(new Record(2000, "Tom"));
	records.push_back(new Record(1500, "Carl"));
	records.push_back(new Record(1000, "Paul"));
}

std::vector<Record*> RecordTable::getRecords() {
	return records;
}
