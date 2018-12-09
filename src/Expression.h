//
// Created by Spencer Tuft on 10/16/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_EXPRESSION_H
#define DATALOG_PARSER_CS236_BYU_EXPRESSION_H

#include <string>
#include <sstream>

class Expression {
 private:
  std::string first;
  std::string second;
  std::string op;
 public:
  Expression() {};
  Expression(std::string firstParam, std::string secondParam, std::string opSymbol)
    : first(firstParam), second(secondParam), op(opSymbol) {};

  void setFirst(std::string operand) {
    first = operand;
  }
  void setSecond(std::string operand) {
    second = operand;
  }
  void setOperator(std::string operatorSymbol) {
    op = operatorSymbol;
  }

  std::string toString() {
    std::stringstream ss;
    ss << "(" << first << op << second << ")";
    return ss.str();
  }
};

#endif //DATALOG_PARSER_CS236_BYU_EXPRESSION_H
