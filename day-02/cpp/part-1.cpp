#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

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

  // Runtime adjustments
  program.at(1) = 12;
  program.at(2) = 2;

  auto pos{0u};
  for (;;) {
    if (pos >= program.size()) { break; }
    const auto opcode{program.at(pos)};
    switch (opcode) {
      case 1:
        program.at(program.at(pos + 3)) = program.at(program.at(pos + 1)) + program.at(program.at(pos + 2));
        pos += 4;
        break;
      case 2:
        program.at(program.at(pos + 3)) = program.at(program.at(pos + 1)) * program.at(program.at(pos + 2));
        pos += 4;
        break;
      case 99:
        std::cout << program.at(0) << std::endl;
        goto loopbreak;
      default:
        throw std::runtime_error("Bad opcode");
    }
  }
loopbreak:

  return EXIT_SUCCESS;
}

