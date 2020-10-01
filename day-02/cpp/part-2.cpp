#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "intcode.hpp"

int main (int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("bad arguments");
  }
  std::vector<int> program;
  {
    auto filename = argv[1];
    std::ifstream infile{filename};
    if (infile.bad() or not infile.is_open()) {
      throw std::runtime_error("cannot open file");
    }

    auto opcode{0};
    for (;;) {
      auto c{infile.get()};
      if (infile.eof()) {
        program.push_back(opcode);
        break; 
      }
      if (c == ',' or c == '\n') {
        program.push_back(opcode);
        opcode = 0;
      } else {
        opcode = 10*opcode + (c - 48);
      }
    }
  }

  const auto copy = program;

  for (int noun = 0; noun < 100; ++noun) {
    for (int verb = 0; verb < 100; ++verb) {
      program = copy;
      // Runtime adjustments
      program.at(1) = noun;
      program.at(2) = verb;

      Intcode computer{program};
      computer.run();
      if (computer.result() == 19690720) { 
        std::cout << noun << " " << verb << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

  return EXIT_SUCCESS;
}

