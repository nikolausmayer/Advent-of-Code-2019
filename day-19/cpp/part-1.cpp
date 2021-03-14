#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
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

  auto result = 0;
  for (auto y = 0; y < 50; y++) {
    for (auto x = 0; x < 50; x++) {
      auto copy = program;
      Intcode intcode{copy};
      intcode.setInput(x);
      intcode.setInput(y);
      intcode.run();
      Intcode::DATA_t tmp;
      while (intcode.getOutput(tmp))
        result += (tmp == 1);
    }
  }
  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

