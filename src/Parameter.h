//
// Created by Spencer Tuft on 10/6/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_PARAMETER_H
#define DATALOG_PARSER_CS236_BYU_PARAMETER_H

#include "Token.h"
#include "./Expression.h"

class Parameter {
 private:
  std::string value;
  std::string type;

 public:
  Parameter() = default;
  explicit Parameter(Token &token): value(token.getValue()), type(token.getType()) {};
  explicit Parameter(Expression expression): value(expression.toString()), type("EXPRESSION") {};
  Parameter(std::string paramValue, std::string paramType): value(std::move(paramValue)), type(std::move(paramType)) {};
  void setValue(std::string paramValue) {
    value = std::move(paramValue);
  }
  void setType(std::string paramType) {
    type = std::move(paramType);
  }
  std::string getValue() {
    return value;
  }
  std::string getType() {
    return type;
  }
};

#endif //DATALOG_PARSER_CS236_BYU_PARAMETER_H
