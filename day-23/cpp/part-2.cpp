
#include <fstream>
#include <iostream>
#include <set>
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

  std::vector<Intcode*> network;
  std::set<Intcode*> working;
  for (Intcode::DATA_t i = 0; i < 50; i++) {
    auto* computer = new Intcode{program};
    computer->setInput(i);
    network.push_back(computer);
    working.insert(computer);
  }

  Intcode::DATA_t NATx = 0, NATy = 0, lasttransmittedNATy = -1;

  while (true) {
    if (working.size() == 0) {
      if (lasttransmittedNATy == NATy) {
        std::cout << lasttransmittedNATy << std::endl;
        break;
      }
      network[0]->setInput(NATx);
      network[0]->setInput(NATy);
      lasttransmittedNATy = NATy;
      working.insert(network[0]);
    }
    
    std::set<Intcode*> new_working;
    for (auto* computer : working) {
      auto res = computer->run();
      Intcode::DATA_t target, x, y;
      while (computer->getOutput(target)) {
        while (not computer->getOutput(x))
          computer->step();
        while (not computer->getOutput(y))
          computer->step();

        if (target == 255) {
          NATx = x;
          NATy = y;
        } else {
          network[target]->setInput(x);
          network[target]->setInput(y);
          new_working.insert(network[target]);
          new_working.insert(computer);
        }
      }
    }
    working = new_working;
  }

  return EXIT_SUCCESS;
}

