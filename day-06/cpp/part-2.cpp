
#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

class Body {
public:
  Body(const std::string& name)
    : name(name)
  { }

  std::string name;
  std::set<std::shared_ptr<Body>> orbiters;
  std::shared_ptr<Body> in_orbit_around{nullptr};
  std::list<std::string> path_to_COM;
};

std::map<std::string, std::shared_ptr<Body>> bodies_map;

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not read file");
  }

  std::string buffer;
  while (std::getline(infile, buffer)) {
    auto split_at = buffer.find(')');
    std::string in_name{buffer.substr(0, split_at)};
    std::string out_name{buffer.substr(split_at + 1, buffer.size() - split_at - 1)};

    const auto find_or_new = [](auto name){
      auto result{bodies_map.find(name)};
      if (result != bodies_map.end()) {
        return result->second;
      } else {
        auto new_body{std::make_shared<Body>(name)};
        bodies_map[name] = new_body;
        return new_body;
      }
    };
    auto body_at_center{find_or_new(in_name)};
    auto orbiting_body{find_or_new(out_name)};
    orbiting_body->in_orbit_around = body_at_center;
    body_at_center->orbiters.insert(orbiting_body);
  }

  //for (auto& [_, body] : bodies_map) {
  //  auto& runner{body->in_orbit_around};
  //  while (runner) {
  //    body->path_to_COM.push_back(runner->name);
  //    runner = runner->in_orbit_around;
  //  }
  //}

  const auto& you = bodies_map["YOU"];
  const auto& santa = bodies_map["SAN"];
  bool found{false};
  {
    auto from_you{0};
    Body* you_runner = &*(you->in_orbit_around);
    while (you_runner and not found) {

      auto from_santa{0};
      Body* santa_runner = &*(santa->in_orbit_around);
      while (santa_runner and not found) {
        //std::cout << "Checking " << you_runner->name << " - " << santa_runner->name << std::endl;
        if (santa_runner->name == you_runner->name) {
          std::cout << "Found handover point " << santa_runner->name << " at " << from_you << "+" << from_santa << "=" << from_you+from_santa << std::endl;
          found = true;
        }
        santa_runner = &*(santa_runner->in_orbit_around);
        //std::cout << santa->in_orbit_around << std::endl;
        ++from_santa;
      }

      you_runner = &*(you_runner->in_orbit_around);
      ++from_you;
    }
  }
}

