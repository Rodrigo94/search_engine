#include "query_processor.h"

QueryProcessor::QueryProcessor(std::string runs_path){
  runs_file.open(runs_path.c_str(), std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
  runs_file_size = runs_file.tellg();
  runs_file.close();
  runs_file.open(runs_path.c_str(), std::ifstream::in | std::ifstream::binary);
  runs_file.seekg(0, runs_file.beg);
  std::cout << "As runs tem tamanho " << runs_file_size << std::endl;
}

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


void QueryProcessor::process_query(std::string query){
  std::istringstream iss(query);
  std::string word;
  while(iss >> word){
    transformed_query.push_back(vocabulary[word]);
  }
  for(auto term_number: transformed_query){
    std::cout << "O term_number e " << term_number << std::endl;
    std::set<uint> term_documents = get_term_documents(term_number);
    occurrencies_set.push_back(term_documents);
  }
}

std::set<uint> QueryProcessor::get_term_documents(uint term_number){
  std::set<uint> term_documents;
  uint vocabulary_size = vocabulary.size();
  long long int max = runs_file_size/16;
  long long int min = 0;
  long long int cut = (max+min)/2;
  long long int diff = max - min;
  uint num, doc, freq, pos;
  char* buffer = new char[16];
  while(diff > 64){
    std::cout << "Cacei na posicao " << 4*cut << std::endl;
    runs_file.seekg(16*cut, runs_file.beg);
    std::cout << 16*cut << std::endl;
    runs_file.read(buffer, 4);
    std::copy(&buffer[0],&buffer[0] + sizeof(uint), reinterpret_cast<char*>(&num));
    std::cout << "Li o term num " << num << std::endl;
    if(num < term_number){
      min = cut;
      cut = (cut + max) / 2;
    }else{
      max = cut;
      cut = (min + cut) / 2;
    }
    diff = max - min;
  }
  while(num != term_number){
    std::cout << "Vai percorrendo: " << num << std::endl;
    runs_file.read(buffer, 16);
    std::copy(&buffer[12],&buffer[15], reinterpret_cast<char*>(&num));
  }
  std::cout << "<" << num << ",";
  while(num == term_number){
    runs_file.read(buffer, 16);
    std::copy(&buffer[0],&buffer[4], reinterpret_cast<char*>(&doc));
    std::cout << doc << ",";
    term_documents.insert(doc);
    std::copy(&buffer[4],&buffer[8], reinterpret_cast<char*>(&freq));
    std::cout << freq << ",";
    std::copy(&buffer[8],&buffer[12], reinterpret_cast<char*>(&pos));
    std::cout << pos << ">" << std::endl;
    std::copy(&buffer[12],&buffer[16], reinterpret_cast<char*>(&num));
    std::cout << "<" << num << ",";
  }
  return term_documents;
}

std::set<uint> QueryProcessor::get_intersections() {
}
