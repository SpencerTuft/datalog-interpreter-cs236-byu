//
// Created by Spencer Tuft on 11/5/18.
//

#include "Interpreter.h"
Interpreter::Interpreter(std::string file_name) {
  DatalogParser dp(file_name);
  s = dp.getSchemes();
  f = dp.getFacts();
  q = dp.getQueries();
  r = dp.getRules();
}
Database Interpreter::build() {
  return Database(s, f, r);
}
std::vector<Scheme> Interpreter::getSchemes(){
  return s;
}
std::vector<Fact> Interpreter::getFacts(){
  return f;
}
std::vector<Query> Interpreter::getQueries() {
  return q;
}
std::vector<Rule> Interpreter::getRules() {
  return r;
}
