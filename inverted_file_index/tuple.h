#ifndef TUPLE_H_
#define TUPLE_H_

#include <fstream>

class Tuple{
private:
  uint term_number_;
  uint document_number_;
  uint frequency_;
  uint position_;
public:
  Tuple(uint term_number, uint document_number, uint frequency, uint position);
  ~Tuple();
  uint TermNumber();
  uint DocumentNumber();
  uint Frequency();
  uint Position();
  void writeTuple(std::ofstream& temp);
  bool sameDocument(Tuple i);
  struct compare{
    bool operator()(const Tuple& i, const Tuple& j){
      if ( i.term_number_ != j.term_number_ )
        return (i.term_number_ < j.term_number_);
      if ( i.document_number_ != j.document_number_)
        return (i.document_number_ < j.document_number_);
      return (i.position_ < j.position_);
    }
  };
};


#endif /* TUPLE_H_ */
