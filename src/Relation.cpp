//
// Created by Spencer Tuft on 10/27/18.
//

#include <algorithm>
#include <sstream>
#include "Relation.h"

Relation::Relation() = default;
Relation::Relation(std::string relation_name) {
  n = std::move(relation_name);
};
Relation::Relation(std::string relation_name, List header) {
  n = std::move(relation_name);
  h = std::move(header);
}
Relation::Relation(std::string relation_name, List header, Tuples tuples) {
  n = std::move(relation_name);
  h = std::move(header);
  for (const auto &tuple : tuples) {
    add(tuple);
  }
}
void Relation::add(Tuples tuples) {
  for (auto &tuple : tuples) {
    add(tuple);
  }
}
void Relation::add(Tuple tuple) {
  if (t.empty()) t.emplace_back(tuple);
  else {
    for (size_t i = 0, max = t.size(); i < max; i++) {
      if (tuple == t[i]) break; // Cannot add duplicates

      if (t[i] > tuple) {
        t.insert(t.begin() + i, tuple);
      } else if (i + 1 == max) {
        t.emplace_back(tuple);
      }
    }
  }
}
void Relation::add(List list) {
  add(Tuple(std::move(list)));
}
void Relation::clear() {
  t.clear();
}
bool Relation::empty() {
  return t.empty();
}
bool Relation::undefined() {
  return h.empty();
}
int Relation::size() {
  return static_cast<int>(t.size());
}
bool Relation::exists(Tuple tuple) {
  for (auto &existing_tuple : t) {
    if (tuple == existing_tuple) return true;
  }
  return false;
}
int Relation::resolve_column(std::string value) {
  for (int i = 0, max = static_cast<int>(h.size()); i < max; i++) {
    if (h[i] == value) return i;
  }
  return -1;
}
std::string Relation::resolve_column(int position) {
  if (position >= 0 && position < static_cast<int>(h.size())) return h[position];
  return "";
}
List Relation::get_header() {
  return h;
}
std::string Relation::get_name() {
  return n;
}
std::vector<Tuple> Relation::get_tuples() {
  return t;
}
Relation Relation::select(int column, std::string value) {
  Relation temp(n, h);
  for (auto &tuple : t) {
    Tuple selected = tuple.select(column, value);
    if (!selected.empty()) temp.add(selected);
  }
  return temp;
}
Relation Relation::select(int column1, int column2) {
  Relation temp(n, h);
  for (auto &tuple : t) {
    Tuple selected = tuple.select(column1, column2);
    if (!selected.empty()) temp.add(selected);
  }
  return temp;
}
Relation Relation::select(std::vector<int> columns) {
  Relation temp(n, h);
  for (auto &tuple : t) {
    Tuple selected = tuple.select(columns);
    if (!selected.empty()) temp.add(selected);
  }
  return temp;
}
Relation Relation::project(List column_names) {
  std::vector<int> column_numbers;
  std::vector<std::string> new_header;

  for (const auto &column_name : column_names) {
    auto num = resolve_column(column_name);
    if (num != -1) {
      column_numbers.emplace_back(num);
      new_header.emplace_back(column_name);
    }
  }

  Relation temp(n, new_header);
  for (auto &tuple : t) {
    Tuple projected = tuple.project(column_numbers);
    temp.add(projected);
  }
  return temp;
}
Relation Relation::project(std::vector<int> column_positions) {
  std::vector<std::string> new_header;
  for (auto &position : column_positions) {
    if (position >= 0 && position < static_cast<int>(h.size())) {
      new_header.emplace_back(h[position]);
    }
  }
  Relation temp(n, new_header);

  for (auto &tuple : t) {
    Tuple projected = tuple.project(column_positions);
    temp.add(projected);
  }
  return temp;
}
Relation Relation::rename(int column, std::string value) {
  List new_header = h;
  new_header[column] = value;
  return Relation(n, new_header, t);
}
Relation Relation::rename(std::string from, std::string to) {
  List new_header = h;
  new_header[resolve_column(from)] = to;
  return Relation(n, new_header, t);
}
Relation Relation::join(std::string name, Relation relation) {
  auto join_on = [](List h1, List h2) {
    std::vector<std::pair<int, int>> join_columns;
    for (size_t i = 0; i < h1.size(); i++) {
      for (size_t j = 0; j < h1.size(); j++) {
        if (h1[i] == h2[j]) {
          std::pair<int, int> temp_pair;
          temp_pair.first = static_cast<int>(i);
          temp_pair.second = static_cast<int>(j);
          join_columns.emplace_back(temp_pair);
        }
      }
    }
    return join_columns;
  };

  auto deduplicate = [](List l1, List l2, std::vector<std::pair<int, int>> column_pairs) {
    // Helper function
    auto includes = [](std::vector<int> vec, int item) {
      for (auto &element : vec) {
        if (element == item) return true;
      }
      return false;
    };

    // Get l2 columns from column_pairs
    std::vector<int> removeFromL2;
    removeFromL2.reserve(column_pairs.size());
    for (auto &pair : column_pairs) {
      removeFromL2.emplace_back(pair.second);
    }

    List deduplicated = l1;
    for (int i = 0, max = static_cast<int>(l2.size()); i < max; i++) {
      if (!includes(removeFromL2, i)) deduplicated.emplace_back(l2[i]);
    }
    return deduplicated;
  };

  auto can_join = [](Tuple &tuple1, Tuple &tuple2, std::vector<std::pair<int, int>> column_pairs) {
    for (auto column_pair : column_pairs) {
      if (tuple1.at(column_pair.first) != tuple2.at(column_pair.second)) {
        return false;
      }
    }
    return true;
  };

  List h1 = this->get_header();
  List h2 = relation.get_header();
  auto join_columns = join_on(h1, h2);
  auto new_header = deduplicate(h1, h2, join_columns);
  Relation temp_relation(std::move(name), new_header);

  for (auto t1 : this->get_tuples()) {
    for (auto t2 : relation.get_tuples()) {
      if (can_join(t1, t2, join_columns)) {
        // Join tuples
        Tuple joined(deduplicate(t1.values(), t2.values(), join_columns));
        // Add new tuple to temp relation
        temp_relation.add(joined);
      }
    }
  }

  return temp_relation;
}
Relation Relation::__union(std::string name, Relation relation) {
  auto union_compatible = [](Relation &r1, Relation r2) {
    auto h1 = r1.get_header();
    auto h2 = r2.get_header();
    for (size_t i = 0, max = h1.size(); i < max; i++) {
      if (h1[i] != h2[i]) return false;
    }
    return true;
  };

  // Check union compatibility
  if (!union_compatible(*this, relation)) return Relation();

  // Add name and header
  Relation combined(std::move(name), this->get_header());

  // Add rows from this relation
  auto rowsA = this->get_tuples();
  for (const auto &row : rowsA) {
    combined.add(row);
  }

  // Add rows from relation passed in
  auto rowsB = relation.get_tuples();
  for (const auto &row : rowsB) {
    combined.add(row);
  }

  return combined;
}
std::string Relation::str() {
  std::stringstream ss;
  for (auto &tuple : t) {
    if (&tuple != &t[0]) ss << std::endl;
    ss << "  " << tuple.str(h);
  }
  return ss.str();
}
