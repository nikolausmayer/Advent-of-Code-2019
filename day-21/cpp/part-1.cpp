#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>
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

  auto step = [&robot](std::string_view s) {
    for (auto c : s)
      robot.setInput(c);
    robot.setInput('\n');
  };

  /// @   x
  /// #####
  ///  ABCD

  /// "Jump if @      or @    "
  ///          #xx.#     #.xxx
  /// i.e. "jump at the first necessary or last possible moment"
  step("NOT C J");
  step("OR D T");
  step("AND T J");
  step("NOT A T");
  step("OR T J");
  step("WALK");

  robot.run();

  Intcode::DATA_t tmp;
  while (robot.getOutput(tmp))
    if (tmp > 255)
      std::cout << (int)tmp << std::endl;
}

