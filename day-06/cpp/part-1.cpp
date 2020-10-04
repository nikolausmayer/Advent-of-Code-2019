
#include <array>
#include <fstream>
#include <iostream>
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
  std::shared_ptr<Body> in_orbit_around;
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

  auto orbits_counter{0};
  for (auto& [name, body] : bodies_map) {
    while (body->in_orbit_around) {
      ++orbits_counter;
      body = body->in_orbit_around;
    }
  }
  std::cout << "Found " << orbits_counter << " orbits" << std::endl;
}

