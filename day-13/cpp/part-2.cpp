
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

  program[0] = 2;
      
  Intcode intcode{program};
  std::map<std::pair<int, int>, int> canvas;
  Intcode::DATA_t ball_x{0}, paddle_x{0};
  Intcode::DATA_t score{0};
  for (;;) {
    auto state{intcode.run()};

    // Output handling
    for (;;) {
      Intcode::DATA_t x, y, value;
      bool ok = intcode.getOutput(x);
      if (not ok) break;
      intcode.getOutput(y);
      intcode.getOutput(value);
      if (x == -1 and y == 0) {
        score = value;
      } else {
        canvas[{x, y}] = value;
      }
      if (value == 3) paddle_x = x;
      if (value == 4) ball_x = x;
    }

    // Input handling
    if (state == Intcode::State::waiting_for_input) {
      if (paddle_x < ball_x)
        intcode.setInput(1);
      else if (paddle_x == ball_x)
        intcode.setInput(0);
      else 
        intcode.setInput(-1);
    }

    auto block_tile_count{0};
    for (const auto& [key, value] : canvas) {
      if (value == 2)
        ++block_tile_count;
    }
    if (block_tile_count == 0 and score != 0)
      break;
  }
  std::cout << "Final score is " << score << std::endl;
}

