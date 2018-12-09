#include <utility>

//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_RULE_H
#define DATALOG_PARSER_CS236_BYU_RULE_H

#include <vector>
#include <sstream>

#include "Predicate.h"

class Rule {
 private:
  Predicate head;
  std::vector<Predicate> list;

 public:
  Rule() = default;
  Rule(Predicate headPredicate): head(std::move(headPredicate)) {};
  Rule(Predicate headPredicate, Predicate predicate) {
    head = std::move(headPredicate);
    list.emplace_back(std::move(predicate));
  };
  Rule(Predicate headPredicate, std::vector<Predicate> predicateList)
    : head(std::move(headPredicate)), list(std::move(predicateList)) {};

  void setHead(Predicate headPredicate) {
    head = std::move(headPredicate);
  }
  void addPredicate(Predicate predicate) {
    list.emplace_back(std::move(predicate));
  }
  void setList(std::vector<Predicate> predicateList) {
    list = std::move(predicateList);
  }

  Predicate getHead() {
    return head;
  }

  std::vector<Predicate> getList() {
    return list;
  }

  std::string toString() {
    std::stringstream ss;
    ss << head.str() << " :- ";
    for (std::size_t i = 0, max = list.size(); i < max; i++) {
      ss << list[i].str();
      if (i != max - 1) ss << ",";
    }
    ss << ".";
    return ss.str();
  }
};

#endif //DATALOG_PARSER_CS236_BYU_RULE_H
