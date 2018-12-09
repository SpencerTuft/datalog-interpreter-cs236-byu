//
// Created by Spencer Tuft on 9/28/18.
//

#ifndef LEX_ANA_CS236_BYU_ROUTES_H
#define LEX_ANA_CS236_BYU_ROUTES_H

#include <cctype>

inline bool rwhitespace(char currentChar) {
  return isspace(currentChar) != 0;
}

inline bool rcolon(char currentChar) {
  return currentChar == ':';
}

inline bool ridentifier(char currentChar) {
  return isalpha(currentChar) != 0;
}

inline bool rperiod(char currentChar) {
  return currentChar == '.';
}

inline bool rcomma(char currentChar) {
  return currentChar == ',';
}

inline bool rqmark(char currentChar) {
  return currentChar == '?';
}

inline bool rmultiply(char currentChar) {
  return currentChar == '*';
}

inline bool radd(char currentChar) {
  return currentChar == '+';
}

inline bool rstring(char currentChar) {
  return currentChar == '\'';
}

inline bool rcomment(char currentChar) {
  return currentChar == '#';
}

inline bool rparen(char currentChar) {
  return currentChar == '(' || currentChar == ')';
}

#endif //LEX_ANA_CS236_BYU_ROUTES_H
