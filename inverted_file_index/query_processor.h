#ifndef QUERY_PROCESSOR_H_
#define QUERY_PROCESSOR_H_

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class QueryProcessor{
private:

  std::ifstream vocabulary_file;
  std::ifstream runs_file;

  std::map<std::string, uint> vocabulary;

  std::vector<uint> transformed_query;
  std::vector<uint> and_query;
  std::vector<uint> or_query;

  std::vector<std::set<uint> > occurrencies_set;

  long long int runs_file_size;
public:
  QueryProcessor(std::string runs_path);
  void process_vocabulary(std::string vocabulary_path);
  void process_query(std::string query);
  std::set<uint> get_term_documents(uint term_number);
  std::set<uint> get_intersections();

};


#endif /* QUERY_PROCESSOR_H_ */
