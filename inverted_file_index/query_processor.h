#ifndef QUERY_PROCESSOR_H_
#define QUERY_PROCESSOR_H_

#include <algorithm>
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
  std::set<uint> transformed_query; // Query of indexes rather than text
  std::vector<std::vector<uint> > occurrences_set;
  std::set<uint> all_ocurrences;
  long long int runs_file_size;
public:
  QueryProcessor(std::string runs_path);
  void process_vocabulary(std::string vocabulary_path);
  void process_query(std::string query);
  std::vector<uint> get_term_documents(uint term_number); // Get document occurrences of a term
  std::vector<uint> get_intersections(); // Get intersection of each vector of documents previously found for each term

};


#endif /* QUERY_PROCESSOR_H_ */
