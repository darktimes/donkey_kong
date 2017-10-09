#include "recordTable.hpp"
#include "utils.hpp"

using namespace Utils;

std::string const * const RecordTable::RECORD_TABLE_FILE =
 new std::string("records");

const unsigned RecordTable::RECORD_COUNT = 5;

Record::Record(unsigned score, std::string playerName): score(score),
  playerName(playerName) {

}

/**
* Either reads the record table, or inits it from defaults, if something
* went wrong
*/
void RecordTable::readTable() {
  std::string* fileString = readFile(*RecordTable::RECORD_TABLE_FILE);
  if (fileString->empty()) {
    Logger::w("Failed to load record table. Initializing from defaults...");
    initDefaults();
  } else if (!parseStringAsRecords(fileString)) {
    Logger::w("Failed to parse record table. Initializing from defaults...");
    initDefaults();
  } else {
    Logger::i("RecordTable succesfully loaded");
  }
}

void RecordTable::saveTable() const {
	std::string recordsString = "";
  for (const auto &record: getRecords()) {
    recordsString += std::to_string(record->score) + "," + record->playerName + ",";
  }
  if (writeFile(*RecordTable::RECORD_TABLE_FILE, recordsString) != 0) {
    Logger::w("Failed to write down the record table");
  }

}

/**
* Parses the given string for records. Returns true, if record table was
* properly populated with given string. Returns false otherwise.
*/
bool RecordTable::parseStringAsRecords(std::string* str) {
	std::string delim(",");
	int start = 0;
	size_t end = str->find(delim);
	int currentScore;
	unsigned i = 0;

	while (end != std::string::npos && i < (RecordTable::RECORD_COUNT * 2)) {
		std::string current_block = str->substr(start, end - start);
		start = end + delim.length();
    end = str->find(delim, start);
		if ( (i % 2) == 0) {
			try {
				currentScore = std::stoi(current_block);
			} catch (...) {
        records.clear();
				return false;
			}
		} else {
      addRecord(new Record(currentScore, current_block));
		}
		i++;
	}
	if (records.size() != RecordTable::RECORD_COUNT) {
    records.clear();
		return false;
	}
	return true;
}

bool RecordTable::isNewScore(unsigned points) const {
	for (auto it = records.begin(); it != records.end(); ++it) {
    if (points >= (*it)->score) {
			return true;
		}
	}
	return false;
}

/**
* Adds record to the record table. Removes the last one, if the number of
* records exceeds the max. number.
*/
void RecordTable::addRecord(Record* record) {
  bool inserted = false;

  for (auto it = records.begin(); it != records.end(); ++it) {
		if (record->score >= (*it)->score) {
			records.insert(it, std::unique_ptr<Record>(record));
      inserted = true;
			break;
		}
	}

  if (records.size() > RecordTable::RECORD_COUNT)  {
    records.pop_back();
  } else if (records.size() < RecordTable::RECORD_COUNT && !inserted) {
    records.push_back(std::unique_ptr<Record>(record));
  }

}

void RecordTable::initDefaults() {
	records.clear();
  addRecord(new Record(3000, "Franklin"));
  addRecord(new Record(2500, "George"));
  addRecord(new Record(2000, "Tom"));
  addRecord(new Record(1500, "Carl"));
  addRecord(new Record(1000, "Paul"));
}

const std::vector<std::unique_ptr<Record>>& RecordTable::getRecords() const {
	return records;
}
