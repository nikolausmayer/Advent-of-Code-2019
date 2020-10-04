
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "intcode.hpp"


int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }

  std::vector<int> program;
  {
    auto filename = argv[1];
    std::ifstream infile{filename};
    if (infile.bad() or not infile.is_open()) {
      throw std::runtime_error("cannot open file");
    }

    auto opcode{0};
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

  Intcode cpu{program};
  cpu.setInput(5);
  auto state = cpu.run();
  if (state != Intcode::State::finished) {
    throw std::runtime_error("finished with error");
  }
  {
    int output;
    while (cpu.getOutput(output)) {
      std::cout << output << std::endl;
    }
  }
  std::cout << "Result: " << cpu.getResult() << std::endl;
}

