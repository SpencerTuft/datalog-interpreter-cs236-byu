//
// Created by Spencer Tuft on 10/29/18.
//

#ifndef RELATIONAL_DATABASE_CS236_BYU_TUPLE_H
#define RELATIONAL_DATABASE_CS236_BYU_TUPLE_H

#include <vector>
#include <string>

typedef std::vector<std::string> List;

class Tuple {
 private:
  List l;
 public:
  Tuple();
  explicit Tuple(List);
  bool operator==(const Tuple&) const;
  bool operator!=(const Tuple&) const;
  bool operator<(const Tuple&) const;
  bool operator>(const Tuple&) const;
  bool empty() const;
  int size() const;
  List values() const;
  std::string at(int) const;
  Tuple select(int, std::string) const;
  Tuple select(int, int) const;
  Tuple select(std::vector<int>) const;
  Tuple project(std::vector<int>) const;
  std::string str(List) const;
};

#endif //RELATIONAL_DATABASE_CS236_BYU_TUPLE_H
