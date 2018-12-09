//
// Created by Spencer Tuft on 10/29/18.
//

#include <algorithm>
#include "Database.h"
Database::Database(std::vector<Scheme> schemes) {
  add(std::move(schemes));
};
Database::Database(std::vector<Scheme> schemes, std::vector<Fact> facts) {
  add(std::move(schemes));
  add(std::move(facts));
}
Database::Database(std::vector<Scheme> schemes, std::vector<Fact> facts, std::vector<Rule> rules) {
  add(std::move(schemes));
  add(std::move(facts));
  add(std::move(rules));
}
void Database::add(std::vector<Relation> relations) {
  for (auto &relation : relations) {
    add(relation);
  }
}
void Database::add(Relation relation) {
  r[relation.get_name()] = relation;
}
Relation Database::evaluate(Predicate predicate) {
  std::string relation_name = predicate.get_id();
  Relation relation = look_up(relation_name);

  relation = stg_select(relation, predicate);
  relation = stg_project(relation, predicate);
  relation = stg_rename(relation, predicate);

  return relation;
}
std::vector<Relation> Database::evaluate(std::vector<Predicate> predicates) {
  std::vector<Relation> relations;
  for (auto &predicate : predicates) {
    relations.emplace_back(evaluate(predicate));
  }
  return relations;
}
std::vector<Relation> Database::execute(std::vector<Query> queries) {
  std::vector<Relation> relations;
  for (auto &query : queries) {
    relations.emplace_back(execute(query));
  }
  return relations;
};
Relation Database::execute(Query query) {
  return evaluate(query.get_predicate());
}
bool Database::exists(std::string relation_name) {
  auto iter = r.find(relation_name);
  return iter != r.end();
}
Relation Database::look_up(std::string relation_name) {
  if (exists(relation_name)) return r.at(relation_name);
  return Relation();
}
std::string Database::str() {
  std::stringstream ss;
  for (auto &relation : r) {
    ss << relation.first << std::endl << relation.second.str() << std::endl;
  }
  return ss.str();
}

// Private methods
Relation Database::stg_select(Relation &relation, Predicate predicate) {
  std::map<int, std::string> constants;
  std::map<std::string, std::vector<int>> variables;
  std::vector<Parameter> params = predicate.get_param_list();
  for (int i = 0, max = static_cast<int>(params.size()); i < max; i++) {
    if (params[i].getType() == "STRING") {
      constants.emplace(i, params[i].getValue());
    }
    if (params[i].getType() == "ID") {
      std::string value = params[i].getValue();
      auto iter = variables.find(value);
      if (iter != variables.end()) {
        variables.at(value).emplace_back(i);
      } else {
        variables.emplace(params[i].getValue(), std::vector<int>{i});
      }
    }
  }

  // Select Constants
  for (auto &constant : constants) {
    relation = relation.select(constant.first, constant.second);
  }

  // Select Variables
  for (auto &variable : variables) {
    std::vector<int> positions = variable.second;
    if (positions.size() > 1) {
      relation = relation.select(positions);
    }
  }
  return relation;
}
Relation Database::stg_project(Relation &relation, Predicate predicate) {
  std::vector<Parameter> params = predicate.get_param_list();

  // Find position of first-encounter, unique variables
  std::vector<int> positions;
  std::vector<std::string> variable_names;
  for (size_t i = 0, max = params.size(); i < max; i++) {
    if (params[i].getType() == "ID") {
      std::string value = params[i].getValue();
      bool found = std::find(variable_names.begin(), variable_names.end(), value) != variable_names.end();
      if (!found) {
        variable_names.emplace_back(value);
        positions.emplace_back(i);
      }
    }
  }

  relation = relation.project(positions);
  return relation;
}
Relation Database::stg_rename(Relation &relation, Predicate predicate) {
  std::vector<Parameter> params = predicate.get_param_list();

  // Find unique variable ordering
  std::vector<std::string> variable_names;
  for (auto &param : params) {
    if (param.getType() == "ID") {
      std::string value = param.getValue();
      bool found = std::find(variable_names.begin(), variable_names.end(), value) != variable_names.end();
      if (!found) variable_names.emplace_back(value);
    }
  }

  for (size_t i = 0, max = variable_names.size(); i < max; i++) {
    relation = relation.rename(static_cast<int>(i), variable_names[i]);
  }

  return relation;
}
void Database::add(Scheme scheme) {
  std::string relation_name = scheme.get_name();
  List headers = scheme.get_list();
  if (!exists(relation_name)) {
    Relation new_relation(relation_name, headers);
    r.emplace(relation_name, new_relation);
  }
}
void Database::add(std::vector<Scheme> schemes) {
  for (auto &scheme : schemes) {
    add(scheme);
  }
}
void Database::add(Fact fact) {
  auto relation_name = fact.get_name();
  if (exists(relation_name)) {
    r.at(relation_name).add(fact.get_list());
  }
}
void Database::add(std::vector<Fact> facts) {
  for (auto &fact : facts) {
    add(fact);
  }
}
void Database::add(Rule rule) {
//  std::cout << "Adding rule: " << rule.toString() << std::endl;

  // Identify rule
  auto head = rule.getHead();
  auto head_name = head.get_id();
  auto predicates = rule.getList();

  // Look up in the database the relation referenced in the head of the rule
  auto match = look_up(head_name);
  if (match.undefined()) return; // Do nothing since the referenced relation does not exist

  // Evaluate predicates on the right side of the rule
  auto relations = evaluate(predicates);

  // Join resulting relations
  Relation result;
  size_t num_relations = relations.size();
  if (num_relations) result = relations.at(0);
  if (num_relations > 1) {
    for (size_t i = 0; i < num_relations; i++) {
      if (i < num_relations - 1) {
        result = result.join(head_name, relations[i + 1]);
      }
    }
  }

  // Project the columns that appear in the head predicate
  auto columns = head.get_param_list_str();
  result = result.project(columns);

  // Rename the relation to make it union-compatible with the original table
  auto match_header = match.get_header();
  for (size_t i = 0, max = match_header.size(); i < max; i++) {
    result = result.rename(static_cast<int>(i), match_header[i]);
  }

  // Union with the relation in the database
  result = match.__union(match.get_name(), result);
  if (!result.undefined()) add(result);
}
void Database::add(std::vector<Rule> rules) {
  auto database_tuple_count = [](auto &r) {
    int count = 0;
    for (auto &relation : r) {
      count += relation.second.get_tuples().size();
    }
    return count;
  };

  int before_tuple_count = database_tuple_count(r);
  int after_tuple_count = 0;
  int iteration_count = 0;
  while (before_tuple_count != after_tuple_count) {
    before_tuple_count = database_tuple_count(r);
    iteration_count++;

    // Extend rules and output results
    for (const auto &rule : rules) {
      add(rule);
    }

    // Evaluate current size of database
    after_tuple_count = database_tuple_count(r);
  }

  // Output Summary
  std::cout << "Schemes populated after " << iteration_count << " passes through the Rules." << std::endl;
}
