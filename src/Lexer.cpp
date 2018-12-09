//
// Created by Spencer Tuft on 9/13/18.
//

#include <vector>
#include <iostream>
#include <sstream>

#include "Lexer.h"
#include "State.h"
#include "Token.h"

bool pstart(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.setLine(inputStream.getLineNumber()); // Set line at beginning of every new token
  return false;
}

std::string route(InputStream inputStream, States states, std::string &currentState) {
  for (auto &state : states) {
    bool routeFound = state.second.runRoute(inputStream.get());
    if (routeFound) {
      return state.first;
    }
  }
  return "UNDEFINED";
}

void Lexer::initializeStates(std::vector<State> inputStates) {
  // Start state is assumed in every lexer
  states.emplace("START", State("START", nullptr, pstart));

  // Map custom input states
  for (auto &state : inputStates) {
    states.emplace(state.getId(), state);
  }
}

Lexer::Lexer() {};

Lexer::Lexer(std::vector<State> inputStates) {
  initializeStates(inputStates);
}

Lexer::Lexer(std::string &fileName, std::vector<State> inputStates) {
  initializeStates(inputStates);
  process(fileName);
}

void Lexer::process(std::string &fileName) {
  // Initialize input stream
  InputStream inputStream = InputStream(fileName);
  // Initialize current Token
  ttemp = Token(inputStream.getLineNumber());

  // Check input stream load orl korrect
  if (!inputStream.isReady()) {
    std::cout << "No input stream loaded" << std::endl;
    return;
  }

  // Analyze the input file one character at a time till the end of file
  while (inputStream.get() != -1) {

    // State Router/Switch
    if (currentState == "START") {
      currentState = route(inputStream, states, currentState);
    }

    // Run process of current state
    bool storeToken = states
        .at(currentState)
        .runProcess(inputStream, ttemp, currentState);

    // Save token and reset
    if (storeToken) {
      tokens.push_back(ttemp); // Store token in the tokens list
      ttemp = Token(inputStream.getLineNumber()); // Reset the current token
    }
  }

  // End of file ritual
  ttemp.set("EOF", "", inputStream.getLineNumber());
  tokens.push_back(ttemp);

  // Set current token to the first token in the array
  ttemp = tokens[0];

  // End of initializations
  lexerReady = true;
}

std::string Lexer::toString() {
  if (lexerReady) {
    std::stringstream ss;
    int listLength = static_cast<int>(tokens.size());

    for (int i = 0; i < listLength; i++) {
      ss << tokens[i].toString();
      ss << std::endl;
    }

    ss << "Total Tokens = " << tokens.size();

    return ss.str();
  } else {
    return "";
  }
}

Token Lexer::currentToken() {
  return tokens[tcursor];
}

void Lexer::advance() {
  tcursor++;
}
