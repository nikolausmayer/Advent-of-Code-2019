
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include "intcode.hpp"


class IntcodeRunner
{
public:
  IntcodeRunner(const auto& program)
    : intcode(program)
  { }
  
  ~IntcodeRunner()
  {
    for (auto& runner : runners) {
      if (runner.joinable()) {
        runner.join();
      }
    }
  }

  void finish() {
    for (auto& runner : runners) {
      if (runner.joinable()) {
        runner.join();
      }
    }
  }

  void takeTheOutputOf(IntcodeRunner& other)
  {
    other.intcode.sendOutputTo(&intcode);
    intcode.getInputFrom(&(other.intcode));
  }

  void run()
  {
    runners.emplace_back([this]{
      intcode.run();
    });
  }

  Intcode intcode;
  std::vector<std::thread> runners;
};


int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not read file");
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


  std::vector<int> phase_inputs{0, 1, 2, 3, 4};

  auto max_output{0};

  do {

    auto program_copy{program};

    IntcodeRunner amp_A{program_copy};
    IntcodeRunner amp_B{program_copy};
    IntcodeRunner amp_C{program_copy};
    IntcodeRunner amp_D{program_copy};
    IntcodeRunner amp_E{program_copy};
    amp_B.takeTheOutputOf(amp_A);
    amp_C.takeTheOutputOf(amp_B);
    amp_D.takeTheOutputOf(amp_C);
    amp_E.takeTheOutputOf(amp_D);

    amp_A.intcode.setInput(phase_inputs[0]);
    amp_B.intcode.setInput(phase_inputs[1]);
    amp_C.intcode.setInput(phase_inputs[2]);
    amp_D.intcode.setInput(phase_inputs[3]);
    amp_E.intcode.setInput(phase_inputs[4]);

    amp_A.intcode.setInput(0);
    amp_A.run();
    amp_B.run();
    amp_C.run();
    amp_D.run();
    amp_E.run();

    amp_A.finish();
    amp_B.finish();
    amp_C.finish();
    amp_D.finish();
    amp_E.finish();

    int final_output{0};
    amp_E.intcode.getOutput(final_output);

    max_output = std::max(max_output, final_output);
  } while (std::next_permutation(phase_inputs.begin(),
                                 phase_inputs.end()));

  std::cout << "Maximum possible output: "
            << max_output << std::endl;
}

