
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

struct Recipe {
  std::map<std::string, int> reqs; 
  std::string output_element;
  int output_quantity;
};
std::ostream& operator<<(std::ostream& os, const Recipe& r) {
  os << "Recipe for " << r.output_quantity << " of "
     << "<" << r.output_element << ">:" << std::endl;
  for (const auto& [k, v] : r.reqs) {
    os << "  " << v << " of " << k << std::endl;
  }
  return os;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::vector<Recipe> all_recipes;
  std::map<std::string, Recipe> all_recipes_map;

  Recipe recipe;
  int quantity{0};
  std::string element;
  enum {
    in_num, in_name, out_num, out_name
  };
  auto state{in_num};
  for (;;) {

    auto c{infile.get()};
    if (infile.eof())
      break;

    switch (c) {
      case '\n': {
        recipe.output_element  = element;
        recipe.output_quantity = quantity;
        quantity = 0;
        all_recipes_map[recipe.output_element] = recipe;
        all_recipes.emplace_back(std::move(recipe));
        recipe = Recipe();
        state = in_num;
        continue;
      }
      case ',': {
        recipe.reqs[element] = quantity;
        quantity = 0;
        state = in_num;
        infile.get();
        continue;
      }
      case ' ': {
        switch (state) {
          case in_num: { 
            element = "";
            state = in_name; 
            continue; 
          }
          case in_name: {
            recipe.reqs[element] = quantity;
            quantity = 0;
            state = out_num; 
            infile.get();  // '='
            infile.get();  // '>'
            infile.get();  // ' '
            continue; 
          }
          case out_num: { 
            state = out_name; 
            element = "";
            continue; 
          }
        }
        continue;
      }
      case '0'...'9': {
        quantity = quantity*10 + (c - 48);
        continue;
      }
      default: {
        element = element + std::string{c};
      }
    }

  }
  //for (const auto& r : all_recipes) {
  //  std::cout << r;
  //}

  unsigned long total_required_ore{0};
  std::stack<std::pair<std::string, int>> todo;
  std::map<std::string, int> stock;
  todo.push({"FUEL", 1});
  while (todo.size()) {
    const auto& [element, quantity] = todo.top();
    //std::cout << std::endl
    //          << "Need " << quantity
    //          << " of " << element << std::endl;

    /// Check if already produced
    if (stock[element] >= quantity) {
      //std::cout << "  already in stock." << std::endl;
      todo.pop();
      continue;
    }

    /// Check requirements
    bool all_in_stock{true};
    for (const auto& [rn, rq] : all_recipes_map[element].reqs) {
      if (rn == "ORE") continue;
      if (stock[rn] < rq) {
        //std::cout << "  " << rq << " " << rn
        //          << " > stock (" << stock[rn] << ")"
        //          << std::endl;
        all_in_stock = false;
        todo.push({rn, rq});
        //std::cout << "  -> " << rn << std::endl;
      }
    }
    if (all_in_stock) {
      for (const auto& [rn, rq] : all_recipes_map[element].reqs) {
        if (rn == "ORE") {
          //std::cout << "  " << rq << " of ORE (" 
          //          << total_required_ore+rq << ")" 
          //          << std::endl;
          total_required_ore += rq;
        } else {
          //std::cout << "  consume " << rq << " of "
          //          << rn << " (" << stock[rn] << " -> "
          //          << stock[rn]-rq << ")" << std::endl;
          stock[rn] -= rq;
        }
      }
      auto unit{all_recipes_map[element].output_quantity};
      //std::cout << "  produce " << unit << " of "
      //          << element << " (" << stock[element]
      //          << " -> " << stock[element]+unit
      //          << ")" << std::endl;
      stock[element] += unit;
      todo.pop();
      continue;
    }

  }
  std::cout << "Total ORE required: " << total_required_ore
            << std::endl;
}

