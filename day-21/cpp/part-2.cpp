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
  /// ##########
  ///  ABCDEFGHI
  ///  123456789
  
  
  /// "Jump if there is a hole in (1,2,3) 
  ///          and ground in 4
  ///          and ground in 5 or 8

  /// Reset T
  step("NOT J T");
  step("AND J T");

  /// not 1 or not 2 or not 3
  step("NOT A J");
  step("NOT B T");
  step("OR T J");
  step("NOT C T");
  step("OR T J");

  /// Reset T
  step("NOT J T");
  step("AND J T");

  /// 4
  step("AND D J");

  /// Reset T
  step("NOT J T");
  step("AND J T");

  /// 5 or 8
  step("OR E T");
  step("OR H T");
  step("AND T J");

  /// Go
  step("RUN");

  robot.run();

  Intcode::DATA_t tmp;
  while (robot.getOutput(tmp))
    if (tmp > 255)
      std::cout << (int)tmp << std::endl;
}

