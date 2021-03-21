#include <fstream>
#include <iostream>
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

  Intcode droid{program};

  auto sendCmd = [&](std::string_view s) {
    for (auto c : s)
      droid.setInput(c);
    droid.setInput('\n');
  };

  std::vector<std::string> sequence{
    "east",
    "take mug",
    "north",
    "take monolith",
    "south",

    "west",
    "west",
    "take ornament",

    "south",
    "east",
    "take weather machine",
    "west",
    "north",
    "west",
    "take astrolabe",
    "south",
    "take hologram",
    
    "north",
    "north",
    "take fuel cell",
    "south",
    "east",
    "east",

    "east",
    "south",
    "west",
    "north",
    "west",
    "take bowl of rice",
    "north",
    "west",
    
    "drop bowl of rice",
    "drop monolith",
    "drop mug",
    "drop weather machine",
    "north",
  };

  for (const auto& cmd : sequence) {
    droid.run();
    Intcode::DATA_t tmp;
    while (droid.getOutput(tmp))
      std::cout << (char)tmp;
    std::cout << std::endl;

    sendCmd(cmd);
    std::cout << "> " << cmd << std::endl;
  }

  droid.run();
  Intcode::DATA_t tmp;
  while (droid.getOutput(tmp))
    std::cout << (char)tmp;
  std::cout << std::endl;

  //while (true) {
  //  droid.run();
  //  Intcode::DATA_t tmp;
  //  while (droid.getOutput(tmp))
  //    std::cout << (char)tmp;
  //  std::cout << std::endl;
  //  std::string input;
  //  std::getline(std::cin, input);
  //  sendCmd(input);
  //}

  return EXIT_SUCCESS;
}

