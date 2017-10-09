#pragma once

#include <string>
#include <vector>
#include <memory>

struct Record {
  unsigned score;
  std::string playerName;
  Record(unsigned score, std::string playerName);
};

class RecordTable {
  static std::string const * const RECORD_TABLE_FILE;

  public:
    static const unsigned int RECORD_COUNT;
    RecordTable() {};
    ~RecordTable() {};
    bool isNewScore(unsigned score) const;
    void addRecord(Record* record);
    void readTable();
    void saveTable() const;
    const std::vector<std::unique_ptr<Record>>& getRecords() const;

  private:
    std::vector<std::unique_ptr<Record>> records;
    void initDefaults();
    bool parseStringAsRecords(std::string* str);
};
