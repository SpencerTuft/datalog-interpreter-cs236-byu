//
// Created by Spencer Tuft on 10/27/18.
//

#ifndef RELATIONAL_DATABASE_CS236_BYU_RELATION_H
#define RELATIONAL_DATABASE_CS236_BYU_RELATION_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "./Tuple.h"

typedef std::set<Tuple> Tuples;

class Relation {
 private:
  std::string n;
  List h;
  Tuples t;
 public:
  Relation();
  explicit Relation(std::string);
  Relation(std::string, List);
  Relation(std::string, List, Tuples);
  void add(Tuples);
  void add(Tuple);
  void add(List);
  void clear();
  bool empty();
  bool undefined();
  int size();
  bool exists(Tuple);
  int resolve_column(std::string);
  std::string resolve_column(int);
  List get_header();
  std::string get_name();
  std::set<Tuple> get_tuples();
  Relation select(int, std::string);
  Relation select(int, int);
  Relation select(std::vector<int>);
  Relation project(List);
  Relation project(std::vector<int>);
  Relation rename(int, std::string);
  Relation rename(std::string, std::string);
  Relation join (std::string, Relation);
  Relation __union(std::string, Relation);
  std::string str();
};

#endif //RELATIONAL_DATABASE_CS236_BYU_RELATION_H
