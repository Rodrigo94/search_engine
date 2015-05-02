#include "query_processor.h"

// Simple constructor, nothing obscure here
QueryProcessor::QueryProcessor(std::string runs_path){
  // Enchantment to find the size of runs file and then properly open it
  runs_file.open(runs_path.c_str(), std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
  runs_file_size = runs_file.tellg();
  runs_file.close();
  runs_file.open(runs_path.c_str(), std::ifstream::in | std::ifstream::binary);
  runs_file.seekg(0, runs_file.beg);
  std::cout << "As runs tem tamanho " << runs_file_size << std::endl;
}

// Create a vocabulary using a file previously set
void QueryProcessor::process_vocabulary(std::string vocabulary_path){
  vocabulary_file.open(vocabulary_path.c_str());
  std::string term;
  uint term_num=0;
  std::cout << "To processando o vocabulario" << std::endl;
  while(vocabulary_file >> term){
    vocabulary[term] = term_num++;
  }
  std::cout << "Processei" << std::endl;
  vocabulary_file.close();
}

// Return the documents that correspond to a query search
void QueryProcessor::process_query(std::string query){
  std::istringstream iss(query);
  std::string word;
  // First, we have to create a vector of indexed word terms.
  while(iss >> word){
    transformed_query.insert(vocabulary[word]);
  }
  // Then we get a vector of documents for each word
  for(auto term_number: transformed_query){
    // Get the documents that correspond to this element
    std::vector<uint> term_documents = get_term_documents(term_number);
    // Insert the vector of documents into a vector of occurences
    occurrences_set.push_back(term_documents);
  }
  std::vector<uint> intersections = get_intersections();
  for(auto it : intersections){
    std::cout << it << " ";
  }
  std::cout << std::endl;
}

// Get a vector of documents for a given term
std::vector<uint> QueryProcessor::get_term_documents(uint term_number){
  std::vector<uint> term_documents;
  long long int max = runs_file_size/16;
  long long int min = 0;
  long long int cut = (max+min)/2;
  long long int diff = max - min;
  uint num, doc, freq, pos;
  char* buffer = new char[16];
  // A binary search for a term in the index
  while(diff > 16){ // While we are not in the position right behind the first occurrence.
    runs_file.seekg(16*cut, runs_file.beg);
    runs_file.read(buffer, 4);
    std::copy(&buffer[0],&buffer[0] + sizeof(uint), reinterpret_cast<char*>(&num));
    if(num < term_number){
      min = cut;
      cut = (cut + max) / 2;
    }else{
      max = cut;
      cut = (min + cut) / 2;
    }
    diff = max - min;
  }
  // Set the position to the first occurence of the term in the index
  while(num != term_number){
    runs_file.read(buffer, 16);
    std::copy(&buffer[12],&buffer[15], reinterpret_cast<char*>(&num));
  }
  // Get all the documents where a term occurs
  while(num == term_number){
    runs_file.read(buffer, 16);
    std::copy(&buffer[0],&buffer[4], reinterpret_cast<char*>(&doc));
    term_documents.push_back(doc);
    std::copy(&buffer[4],&buffer[8], reinterpret_cast<char*>(&freq));
    std::copy(&buffer[8],&buffer[12], reinterpret_cast<char*>(&pos));
    std::copy(&buffer[12],&buffer[16], reinterpret_cast<char*>(&num));
  }
  return term_documents;
}

// Get the intersection of all vectors of occurences.
std::vector<uint> QueryProcessor::get_intersections(){
  // The first intersection will be the first vector, of course
  std::vector<uint> intersections;
  intersections.swap(occurrences_set[0]);
  for(uint i = 1; i<occurrences_set.size(); i++){
    std::vector<uint> temp;
    std::set_intersection(intersections.begin(), intersections.end(),
         occurrences_set[i].begin(), occurrences_set[i].end(), std::back_inserter(temp));
    intersections.swap(temp);
  }
  return intersections;
}
