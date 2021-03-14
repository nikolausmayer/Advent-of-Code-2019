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

  auto in_beam = [&](int x, int y) {
    auto copy = program;
    Intcode intcode{copy};
    intcode.setInput(x);
    intcode.setInput(y);
    intcode.run();
    Intcode::DATA_t tmp = 0;
    intcode.getOutput(tmp);
    return (tmp == 1);
  };

  //for (auto y = 0; y < 50; y++) {
  //  for (auto x = 0; x < 50; x++) {
  //    std::cout << (in_beam(x,y) ? '#' : '.');
  //  }
  //  std::cout << std::endl;
  //}
  //std::cout << std::endl;


  auto x = 5;
  auto y = 4;
  while (true) {
    y++;
    while (in_beam(x, y)) {
      y++;
    }
    while (not in_beam(x, y)) {
      /// Huh this is necessary. I guess there is a memory problem in the Intcode...?
      in_beam(x,y);
      x++;
    }
    bool ok = true;
    for (auto i = 0; i < 100; i++)
      if (not in_beam(x+i, y-i))
        ok = false;
    if (ok)
      break;
  }
  std::cout << y-99 + 10000*x << std::endl;


  return EXIT_SUCCESS;
}

