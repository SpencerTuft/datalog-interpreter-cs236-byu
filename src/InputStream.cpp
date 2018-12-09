//
// Created by Spencer Tuft on 9/17/18.
//

#include "InputStream.h"
InputStream::InputStream(std::string &fileName) {
  std::ifstream ifs(fileName);
  if (ifs.is_open()) {
    while (ifs.peek() != EOF) {
      char c;
      ifs.get(c);
      str.push_back(c);
    }

    ifs.close();
    str.push_back(-1);
    r = true;
  }
}
char InputStream::get() const {
  return str[charLoc];
}
char InputStream::peek() const {
  return str[charLoc+1];
}
void InputStream::forward() {
  if (str[charLoc] == '\n') {
    currentLineNum++;
  }
  charLoc++;

}
void InputStream::forward(int num) {
  for (int i = 0; i < num; i ++) {
    forward();
  }
}
int InputStream::getLineNumber() const {
  return currentLineNum;
}
int InputStream::getCharacterPosition() const {
  return charLoc;
}
bool InputStream::isReady() const {
  return r;
}
std::string InputStream::toString() const {
  return str;
}
