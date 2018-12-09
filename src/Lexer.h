//
// Created by Spencer Tuft on 9/13/18.
//

#ifndef LEX_ANA_CS236_BYU_Lexer_H
#define LEX_ANA_CS236_BYU_Lexer_H

#include <functional>
#include <vector>
#include <iostream>
#include <map>

#include "Token.h"
#include "State.h"
#include "InputStream.h"

typedef std::map<std::string, State> States;

class Lexer {
 private:
  bool lexerReady = false;
  std::string currentState = "START";
  States states;
  Token ttemp;
  std::vector<Token> tokens;
  int tcursor = 0;

 public:
  explicit Lexer();
  explicit Lexer(std::vector<State> inputStates);
  explicit Lexer(std::string &fileName, std::vector<State> inputStates);
  void initializeStates(std::vector<State> inputStates);
  void process(std::string &fileName);
  std::string toString();
  Token currentToken();
  void advance();
};

#endif //LEX_ANA_CS236_BYU_Lexer_H
