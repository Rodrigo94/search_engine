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

  std::vector<std::set<uint> > set_of_terms_appearances_list;
public:
  QueryProcessor(std::string vocabulary_path, std::string runs_path);
  void process_vocabulary();
  void process_query(std::string query);
  void search_term_in_run(uint term_number);
  std::set<uint> get_term_appearances(uint term_number);
  std::set<uint> get_intersections();

};


#endif /* QUERY_PROCESSOR_H_ */
