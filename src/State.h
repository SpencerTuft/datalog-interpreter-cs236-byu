//
// Created by Spencer Tuft on 9/21/18.
//

#ifndef LEX_ANA_CS236_BYU_STATE_H
#define LEX_ANA_CS236_BYU_STATE_H

#include <string>
#include "InputStream.h"
#include "Token.h"

typedef bool (Process)(InputStream &, Token &, std::string &);
typedef bool (Route)(char);

class State {
 private:
  std::string id;
  Route* r;
  Process* p;

 public:
  explicit State(std::string identifier, Route* route, Process* process)
      : id(std::move(identifier)), r(route), p(process) {}

  bool runProcess(InputStream &inputStream, Token &token, std::string &currentState) {
    if (p != nullptr) {
      return (*p)(inputStream, token, currentState);
    }
    return false;
  }
  
  bool runRoute(char currentCharacter) {
    if (r != nullptr) {
      return (*r)(currentCharacter);
    }
    return false;
  }

  std::string getId() {
    return id;
  }
};

#endif //LEX_ANA_CS236_BYU_STATE_H
