#include <utility>

//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_FACT_H
#define DATALOG_PARSER_CS236_BYU_FACT_H

#include <string>
#include <sstream>
#include <vector>
#include "Token.h"

class Fact {
 private:
  std::string name;
  std::vector<std::string> list;

 public:
  Fact() = default;
  Fact(std::string fact_name) : name(std::move(fact_name)) {};
  Fact(std::string fact_name, std::string value) {
    name = fact_name;
    list.emplace_back(value);
  };
  Fact(std::string fact_name, std::vector<std::string> values) : name(std::move(fact_name)), list(std::move(values)) {};

  void add(std::string value) {
    list.emplace_back(value);
  }

  void set_name(std::string factName) {
    name = factName;
  }

  std::string get_name() {
    return name;
  }

  std::vector<std::string> get_list() {
    return list;
  }

  std::string str() {
    std::stringstream ss;
    ss << name << "(";
    for (std::size_t i = 0, max = list.size(); i < max; i++) {
      ss << list[i];
      if (i != max - 1) ss << ",";
    }
    ss << ").";
    return ss.str();
  };
};

#endif //DATALOG_PARSER_CS236_BYU_FACT_H
