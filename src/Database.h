//
// Created by Spencer Tuft on 10/29/18.
//

#ifndef RELATIONAL_DATABASE_CS236_BYU_DATABASE_H
#define RELATIONAL_DATABASE_CS236_BYU_DATABASE_H

#include <map>
#include <sstream>
#include "./DatalogParser.h"
#include "./Relation.h"

class Database {
 private:
  std::map<std::string, Relation> r;
  Relation stg_select(Relation&, Predicate);
  Relation stg_project(Relation&, Predicate);
  Relation stg_rename(Relation&, Predicate);
  void add(std::vector<Scheme>);
  void add(Scheme);
  void add(std::vector<Fact>);
  void add(Fact);
  void add(std::vector<Rule>);
  void add(Rule);
 public:
  Database(std::vector<Scheme>);
  Database(std::vector<Scheme>, std::vector<Fact>);
  Database(std::vector<Scheme>, std::vector<Fact>, std::vector<Rule>);
  void add(std::vector<Relation>);
  void add(Relation);
  std::vector<Relation> evaluate(std::vector<Predicate>);
  Relation evaluate(Predicate);
  std::vector<Relation> execute(std::vector<Query>);
  Relation execute(Query);
  bool exists(std::string);
  Relation look_up(std::string);
  std::string str();
};

#endif //RELATIONAL_DATABASE_CS236_BYU_DATABASE_H
