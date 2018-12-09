#include <utility>

//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_SCHEMES_H
#define DATALOG_PARSER_CS236_BYU_SCHEMES_H

#include <vector>
#include <string>
#include <sstream>

#include "Token.h"

class Scheme {
 private:
  std::string name;
  std::vector<std::string> list;
 public:
  Scheme() = default;
  Scheme(std::string scheme_name): name(std::move(scheme_name)) {};
  Scheme(std::string scheme_name, std::string id) {
    name = scheme_name;
    list.emplace_back(id);
  };
  Scheme(std::string scheme_name, std::vector<std::string> id_list): name(std::move(scheme_name)), list(std::move(id_list)) {};

  void setName(std::string scheme_name) {
    name = scheme_name;
  }

  void addId(std::string identifier) {
    list.push_back(identifier);
  }

  std::vector<std::string> get_list() {
    return list;
  };

  std::string get_name() {
    return name;
  };

  std::string str() {
    std::stringstream ss;
    ss << name << "(";
    for (std::size_t i = 0, max = list.size(); i < max; i++) {
      ss << list[i];
      if (i != max - 1) ss << ",";
    }
    ss << ")";
    return ss.str();
  };
};

#endif //DATALOG_PARSER_CS236_BYU_SCHEMES_H
