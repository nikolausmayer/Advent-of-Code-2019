
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "intcode.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  std::vector<Intcode::DATA_t> program;
  {
    auto filename = argv[1];
    std::ifstream infile{filename};
    if (infile.bad() or not infile.is_open()) {
      throw std::runtime_error("cannot open file");
    }
    Intcode::DATA_t opcode{0};
    auto negative{false};
    for (;;) {
      auto c{infile.get()};
      if (infile.eof()) {
         program.push_back(negative ? -opcode : opcode);
        break; 
      }
       if (c == ',' or c == '\n') {
         program.push_back(negative ? -opcode : opcode);
        opcode = 0;
        negative = false;
      } else if (c == '-') {
        negative = true;
      } else {
         opcode = 10*opcode + (c - 48);
      }
    }
  }

  Intcode robot{program};
  robot.run();

  std::vector<std::string> map;
  //auto print_map = [&map] {
  //  for (const auto& line : map)
  //    std::cout << line << std::endl;
  //};

  {
    map.push_back("");

    Intcode::DATA_t pixel;
    while (robot.getOutput(pixel)) {
      if (pixel == '\n')
        map.push_back("");
      else
        map.back() = map.back() + std::string{(char)pixel};

      if (pixel == 'X')
        throw std::runtime_error("Robot fell off scaffold!");
    }
    map.pop_back();
    map.pop_back();
  }

  /// Find (and mark) intersections
  auto isect_sum{0u};
  for (auto y{1u}; y < map.size()-1; ++y) {
    for (auto x{1u}; x < map[y].size()-1; ++x) {
      if (map[y  ][x  ] == '#' and
          map[y-1][x  ] == '#' and
          map[y+1][x  ] == '#' and
          map[y  ][x-1] == '#' and
          map[y  ][x+1] == '#') {
        //map[y][x] = 'O';
        isect_sum += x*y;
      }
    }
  }

  //print_map();
  std::cout << "Sum of alignment parameters is "
            << isect_sum << std::endl;
}

