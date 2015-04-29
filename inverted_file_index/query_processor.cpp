#include "query_processor.h"

QueryProcessor::QueryProcessor(std::string vocabulary_path, std::string runs_path){
  vocabulary_file.open(vocabulary_path.c_str());
  process_vocabulary();
  vocabulary_file.close();
  runs_file.open(runs_path.c_str(), std::ifstream::in | std::ifstream::binary);
  runs_file.seekg(0, runs_file.beg);
}

void QueryProcessor::process_vocabulary(){
  std::string term;
  uint term_num=0;
  while(vocabulary_file >> term){
    vocabulary[term] = term_num++;
  }
}


void QueryProcessor::process_query(std::string query){
  std::istringstream iss(query);
  std::string word;
  while(iss >> word){
    transformed_query.push_back(vocabulary[word]);
  }
  for(auto term_number: transformed_query){
    search_term_in_run(term_number);
    std::set<uint> appearances_set = get_term_appearances(term_number);
    set_of_terms_appearances_list.push_back(appearances_set);
  }
  std::set<uint> intersections = get_intersections();
}

void QueryProcessor::search_term_in_run(uint term_number) {
}

std::set<uint> QueryProcessor::get_term_appearances(uint term_number) {
}

std::set<uint> QueryProcessor::get_intersections() {
}
