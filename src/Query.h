#include <utility>



//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_QUERY_H
#define DATALOG_PARSER_CS236_BYU_QUERY_H

#include <sstream>
#include "Predicate.h"

class Query {
 private:
  Predicate pred;

 public:
  Query() = default;
  Query(Predicate predicate): pred(std::move(predicate)) {};

  void set_predicate(Predicate predicate) {
    pred = std::move(predicate);
  }

  Predicate get_predicate() {
    return pred;
  }

  std::string str() {
    std::stringstream ss;
    ss << pred.str() << "?";
    return ss.str();
  }
};

#endif //DATALOG_PARSER_CS236_BYU_QUERY_H
