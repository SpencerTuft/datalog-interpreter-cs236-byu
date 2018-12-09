//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_DATALOGPARSER_H
#define DATALOG_PARSER_CS236_BYU_DATALOGPARSER_H

#include <vector>

#include "./Scheme.h"
#include "./Fact.h"
#include "./Rule.h"
#include "./Query.h"
#include "DatalogLexer.h"

class DatalogParser {
 private:
  std::vector<Scheme> schemes;
  std::vector<Fact> facts;
  std::vector<Rule> rules;
  std::vector<Query> queries;
  std::vector<std::string> domain;
  DatalogLexer lexer;

  Token requireType(std::string type);
  Token requireType(std::vector<std::string> types);

  std::vector<Scheme> createSchemes();
  Scheme createScheme();

  std::vector<Fact> createFacts();
  Fact createFact();

  std::vector<Rule> createRules();
  Rule createRule();

  std::vector<Query> createQueries();
  Query createQuery();

  std::vector<std::string> createDomain();

  Predicate createHeadPredicate();
  Predicate createPredicate();
  Parameter createParameter();
  Expression createExpression();

 public:
  explicit DatalogParser(std::string& fileName);
  std::string toString();
  std::vector<Scheme> getSchemes();
  std::vector<Fact> getFacts();
  std::vector<Rule> getRules();
  std::vector<Query> getQueries();
  std::vector<std::string> getDomain();
};

#endif //DATALOG_PARSER_CS236_BYU_DATALOGPARSER_H
