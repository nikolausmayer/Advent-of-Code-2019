
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>
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
      
  Intcode intcode{program};
  intcode.run();
  std::map<std::pair<int, int>, int> canvas;
  for (;;) {
    Intcode::DATA_t x, y, value;
    bool ok = intcode.getOutput(x);
    if (not ok) break;
    intcode.getOutput(y);
    intcode.getOutput(value);
    canvas[{x, y}] = value;
  }
  auto block_tile_count{0};
  for (const auto& [key, value] : canvas) {
    if (value == 2)
      ++block_tile_count;
  }
  std::cout << "There are " << block_tile_count
            << " block tiles on screen." << std::endl;
}

