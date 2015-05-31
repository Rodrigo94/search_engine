/*
 * Query Processor
 * It must me able to query the index created for a specific collection
 * There must exist a index file of ordered tuples following the rule:
 * <term number, document number, frequency, position> all integers starting from 0 to MAX_INTEGER
 * term_number and document_number are integers that must have no gap greater than 1
 * The vocabulary file must be a plain/text file that have all the terms in the document in order of appearance and '\n' separated
 *
 * Usage
 * First one should construct the query processor with the basic constructor. The parameter is the full path of the index
 * Then one must create the vocabulary with process_vocabulary() passing the full path of the vocabulary (This may take a while)
 * Finally it is possible process any query with process_query() using any string as parameter.
 *
 * Query Processing
 * There is a reserved word for the query, the word 'or' which means that a query can be divided as:
 * t11 ... t1n  or ... or tm1 ... tmn | Where tij is the jth term of 'or block' i
 *
 * If the term is not at the vocabulary, the query considers it as 1<<30 (INVALID), so it is assumed that there is less
 * than 1<<30 terms in the vocabulary
 */

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

#define INVALID 1<<30

class QueryProcessor{
private:
  std::string index_file_full_path;
  std::ifstream vocabulary_file;
  std::ifstream runs_file;
  std::map<std::string, uint> vocabulary;
  long long int runs_file_size;
  std::vector<uint> process_index_terms(std::set<uint> index_terms);
  std::vector<uint> get_term_documents(uint term_number); // Get document occurrences of a term
  std::vector<uint> get_intersections(std::vector<std::vector<uint> > occurrences_set); // Get intersection of each vector of documents previously found for each term

public:
  QueryProcessor(std::string index_file_full_path); // Constructor
  void process_vocabulary(std::string vocabulary_full_path); // Prepare Vocabulary
  void process_query(std::string query); // Process query

};


#endif /* QUERY_PROCESSOR_H_ */
