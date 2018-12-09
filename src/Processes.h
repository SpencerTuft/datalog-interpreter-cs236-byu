//
// Created by Spencer Tuft on 9/18/18.
//

#ifndef LEX_ANA_CS236_BYU_DATALOG_H
#define LEX_ANA_CS236_BYU_DATALOG_H

#include <iostream>
#include <string>
#include "Token.h"
#include "InputStream.h"

inline bool pwhitespace(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  inputStream.forward(); // Skip whitespace
  currentToken.setLine(inputStream.getLineNumber()); // Retrieve the current token line number
  currentState = "START";
  return false;
}

inline bool pcolon(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  if (inputStream.peek() == '-') {
    currentToken.set("COLON_DASH", ":-");
    inputStream.forward(2);
  } else {
    currentToken.set("COLON", ":");
    inputStream.forward();
  }
  currentState = "START";
  return true;
}

inline bool pperiod(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("PERIOD", ".");
  inputStream.forward();
  currentState = "START";
  return true;
}

inline bool pcomma(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("COMMA", ",");
  inputStream.forward();
  currentState = "START";
  return true;
}

inline bool pqmark(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("Q_MARK", "?");
  inputStream.forward();
  currentState = "START";
  return true;
}

inline bool pmultiply(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("MULTIPLY", "*");
  inputStream.forward();
  currentState = "START";
  return true;
}

inline bool padd(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("ADD", "+");
  inputStream.forward();
  currentState = "START";
  return true;
}

inline bool pstring(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  char quote = '\'';

  // Current token is a quote
  currentToken.setType("STRING");
  currentToken.addValue(inputStream.get());
  inputStream.forward();

  while (inputStream.get() != quote) {
    if (inputStream.get() == -1) {
      currentToken.setType("UNDEFINED");
      return true;
    }
    currentToken.addValue(inputStream.get());
    inputStream.forward();
  }

  currentToken.addValue(inputStream.get());
  inputStream.forward();
  currentState = "POSSIBLE_STRING_END";
  return false;
}

inline bool ppstring(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  char quote = '\'';
  if (inputStream.get() == quote) { // Found an apostrophe
    currentState = "STRING"; // Back to the string state
    return false; // Continue without creating string token
  } else { // Found string end
    currentState = "START"; // Will return to starting state
    return true; // new string token will be created
  }
}

inline bool pidentifier(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  // Set identifier state
  currentToken.setType("ID");

  // Current and all following characters that are letters or numbers
  do {
    currentToken.addValue(inputStream.get());
    inputStream.forward();
  } while (isalnum(inputStream.get()));

  // Check if identifier is a special identifier
  std::string value = currentToken.getValue();
  if (value == "Schemes") {
    currentToken.setType("SCHEMES");
  } else if (value == "Facts") {
    currentToken.setType("FACTS");
  } else if (value == "Rules") {
    currentToken.setType("RULES");
  } else if (value == "Queries") {
    currentToken.setType("QUERIES");
  }

  currentState = "START";
  return true;
}

inline bool pcomment(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  char bar = '|';
  char hash = '#';
  std::string barhash = "|#";
  char newline = '\n';
  char end = -1;

  // Beginning of comment ritual
  inputStream.forward();

  if (inputStream.get() != bar) { // Is single-line comment
    while (inputStream.get() != newline && inputStream.get() != end) { // Check for end of line or file
      inputStream.forward();
    }
  } else { // Is multi-line comment
    inputStream.forward();

    while (!(inputStream.get() == bar && inputStream.peek() == hash)) {
      // Check for end of file
      if (inputStream.get() == end) {
        currentToken.setType("UNDEFINED");
        return true;
      }
      // Add any other symbols to token value
      inputStream.forward();
    }

    // End of multi-line comment ritual
    inputStream.forward(2);
  }

  currentState = "START";
  return false;
}

inline bool pparen(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  char cc = inputStream.get();
  if (cc == '(') {
    currentToken.set("LEFT_PAREN", cc);
  } else {
    currentToken.set("RIGHT_PAREN", cc);
  }
  inputStream.forward();

  currentState = "START";
  return true;
}

inline bool pund(InputStream &inputStream, Token &currentToken, std::string &currentState) {
  currentToken.set("UNDEFINED", inputStream.get());
  inputStream.forward();

  currentState = "START";
  return true;
}

#endif //LEX_ANA_CS236_BYU_DATALOG_H
