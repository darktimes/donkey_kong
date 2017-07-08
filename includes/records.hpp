#pragma once

#include <iostream>
#include <string.h>
#include <vector>
namespace Engine {
	struct Record {
		int score;
		std::string playerName;
		Record(int score, std::string playerName);
	};

	class RecordTable {
		static std::string const * const RECORD_TABLE_FILE;
		
		public:
			static const unsigned int RECORD_COUNT;
			RecordTable();
			~RecordTable();
			bool isNewScore(int score);
			void addRecord(Record* record);
			void readTable();
			void saveTable();
			std::vector<Record*> getRecords();
		private:
			std::vector<Record*> records; 
			void initDefaults();
			bool parseStringAsRecords(std::string& str);
	};
}
