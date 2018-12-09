//
// Created by Spencer Tuft on 10/6/18.
//

#include <sstream>
#include <set>
#include "./DatalogParser.h"
#include "./Scheme.h"
#include "Token.h"
#include "DatalogParser.h"

DatalogParser::DatalogParser(std::string &fileName) {
  lexer.process(fileName);
  try {
    schemes = createSchemes();
    facts = createFacts();
    rules = createRules();
    queries = createQueries();
    requireType("EOF");

    domain = createDomain();
  } catch (Token &error) {
    std::cout << "Failure!" << std::endl << "  " << error.toString() << std::endl;
  }
}

std::vector<Scheme> DatalogParser::getSchemes() {
  return schemes;
}

std::vector<Fact> DatalogParser::getFacts() {
  return facts;
}

std::vector<Rule> DatalogParser::getRules() {
  return rules;
}

std::vector<Query> DatalogParser::getQueries() {
  return queries;
}

std::vector<std::string> DatalogParser::getDomain() {
  return domain;
}

Token DatalogParser::requireType(std::string type) {
  Token curToken = lexer.currentToken();
  if (!curToken.ofType(std::move(type))) throw curToken;
  lexer.advance();
  return curToken;
}

Token DatalogParser::requireType(std::vector<std::string> types) {
  Token curToken = lexer.currentToken();
  for (auto &type : types) {
    if (curToken.ofType(std::move(type))) {
      lexer.advance();
      return curToken;
    };
  }
  throw curToken;
}

std::vector<Scheme> DatalogParser::createSchemes() {
  requireType("SCHEMES");
  requireType("COLON");

  std::vector<Scheme> newSchemes;
  do {
    newSchemes.emplace_back(createScheme());
  } while (lexer.currentToken().ofType("ID"));

  return newSchemes;
}

Scheme DatalogParser::createScheme() {
  std::string schemeName = requireType("ID").getValue(); // Name scheme
  requireType("LEFT_PAREN"); // Get scheme id list
  std::string schemeId = requireType("ID").getValue();
  Scheme newScheme(schemeName, schemeId);

  while (lexer.currentToken().ofType("COMMA")) {
    lexer.advance(); // Skip comma
    newScheme.addId(requireType("ID").getValue());
  }
  requireType("RIGHT_PAREN");
  return newScheme;
}

std::vector<Fact> DatalogParser::createFacts() {
  requireType("FACTS");
  requireType("COLON");
  std::vector<Fact> newFacts;
  while (lexer.currentToken().ofType("ID")) {
    newFacts.emplace_back(createFact());
  }
  return newFacts;
}

Fact DatalogParser::createFact() {
  std::string factName = requireType("ID").getValue(); // Name fact
  requireType("LEFT_PAREN"); // Get fact string list
  std::string factValue = requireType("STRING").getValue();
  Fact newFact(factName, factValue);
  while (lexer.currentToken().ofType("COMMA")) {
    lexer.advance(); // Skip comma
    newFact.add(requireType("STRING").getValue());
  }
  requireType("RIGHT_PAREN");
  requireType("PERIOD");
  return newFact;
}

std::vector<Rule> DatalogParser::createRules() {
  requireType("RULES");
  requireType("COLON");
  std::vector<Rule> newRules;
  while (lexer.currentToken().ofType("ID")) {
    newRules.emplace_back(createRule());
  }
  return newRules;
}

Rule DatalogParser::createRule() {
  Predicate headPredicate = createHeadPredicate();
  requireType("COLON_DASH");
  Rule newRule(headPredicate, createPredicate());
  while (lexer.currentToken().ofType("COMMA")) {
    lexer.advance(); // Skip comma
    newRule.addPredicate(createPredicate());
  };
  requireType("PERIOD");
  return newRule;
}

Predicate DatalogParser::createHeadPredicate() {
  std::string predicateId = requireType("ID").getValue();
  requireType("LEFT_PAREN");
  std::string parameterValue = requireType("ID").getValue();
  Predicate newHeadPredicate(predicateId, Parameter(parameterValue, "ID"));
  while (lexer.currentToken().ofType("COMMA")) {
    lexer.advance(); // Skip comma
    parameterValue = requireType("ID").getValue();
    newHeadPredicate.add_param(Parameter(parameterValue, "ID"));
  }
  requireType("RIGHT_PAREN");
  return newHeadPredicate;
}

Predicate DatalogParser::createPredicate() {
  std::string predicateId = requireType("ID").getValue();
  requireType("LEFT_PAREN");
  Predicate newPredicate(predicateId, createParameter());
  while (lexer.currentToken().ofType("COMMA")) {
    lexer.advance(); // Skip comma
    newPredicate.add_param(createParameter());
  }
  requireType("RIGHT_PAREN");
  return newPredicate;
}

Parameter DatalogParser::createParameter() {
  std::vector<std::string> expression{"STRING", "ID", "LEFT_PAREN"};
  Token param = requireType(expression);

  if (param.getType() == "LEFT_PAREN") {
    return Parameter(createExpression());
  } else {
    return Parameter(param);
  }
}

Expression DatalogParser::createExpression() {
  Expression newExpression;

  // First parameter
  Token param = requireType(std::vector<std::string>{"STRING", "ID", "LEFT_PAREN"});
  if (param.ofType("LEFT_PAREN")) newExpression.setFirst(createExpression().toString());
  else newExpression.setFirst(param.getValue());

  // operator
  param = requireType(std::vector<std::string>{"ADD", "MULTIPLY"});
  newExpression.setOperator(param.getValue());

  // Second parameter
  param = requireType(std::vector<std::string>{"STRING", "ID", "LEFT_PAREN"});
  if (param.ofType("LEFT_PAREN")) newExpression.setSecond(createExpression().toString());
  else newExpression.setSecond(param.getValue());

  requireType("RIGHT_PAREN");

  return newExpression;
}

std::vector<Query> DatalogParser::createQueries() {
  requireType("QUERIES");
  requireType("COLON");
  std::vector<Query> newQueries;
  do {
    newQueries.emplace_back(createQuery());
  } while (lexer.currentToken().ofType("ID"));
  return newQueries;
}

Query DatalogParser::createQuery() {
  Query newQuery(createPredicate());
  requireType("Q_MARK");
  return newQuery;
}

std::vector<std::string> DatalogParser::createDomain() {
  std::set<std::string> domain;

  // Copy values to set
  for (auto &fact : facts) {
    std::vector<std::string> items = fact.get_list();
    for (auto &item : items) {
      domain.emplace(item);
    }
  }

  // Convert back to vector form
  std::vector<std::string> unique(domain.begin(), domain.end());
  return unique;
}

std::string DatalogParser::toString() {
  std::stringstream ss;

  ss << "Schemes(" << schemes.size() << "):";
  for (auto &scheme : schemes) {
    ss << std::endl << "  " << scheme.str();
  }

  ss << std::endl;

  ss << "Facts(" << facts.size() << "):";
  for (auto &fact : facts) {
    ss << std::endl << "  " << fact.str();
  }

  ss << std::endl;

  ss << "Rules(" << rules.size() << "):";
  for (auto &rule : rules) {
    ss << std::endl << "  " << rule.toString();
  }

  ss << std::endl;

  ss << "Queries(" << queries.size() << "):";
  for (auto &query : queries) {
    ss << std::endl << "  " << query.str();
  }

  ss << std::endl;

  ss << "Domain(" << domain.size() << "):";
  for (auto &item : domain) {
    ss << std::endl << "  " << item;
  }

  return ss.str();
}
