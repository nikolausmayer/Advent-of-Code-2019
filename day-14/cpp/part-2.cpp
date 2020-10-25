
#include <fstream>
#include <iomanip>
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

  /// Compute fractional unit cost per element
  std::map<std::string, double> element_cost_map;
  element_cost_map["ORE"] = 1.;
  all_recipes_map.erase(all_recipes_map.find("ORE")->first);

  while (all_recipes_map.size()) {
    for (const auto& [el, recipe] : all_recipes_map) {
      bool incomplete{false};
      double unit_price{0.};
      for (const auto& [req, num] : recipe.reqs) {
        if (element_cost_map.find(req) ==
            element_cost_map.end()) {
          incomplete = true;
          break;
        } else {
          unit_price += num * element_cost_map[req];
        }
      }
      if (incomplete)
        continue;

      element_cost_map[el] = unit_price/recipe.output_quantity;
      //std::cout << "Unit price for " << el
      //          << " is " << element_cost_map[el]
      //          << std::endl;
      all_recipes_map.erase(all_recipes_map.find(el)->first);
      break;
    }
  }

  std::cout << "1.000.000.000.000 ORE can produce "
            << static_cast<unsigned long>(1000000000000 / element_cost_map["FUEL"])
            << " FUEL." << std::endl;
}

