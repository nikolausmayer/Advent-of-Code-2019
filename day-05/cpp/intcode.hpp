#include <experimental/array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include <string_view>

class Intcode
{
public:
  Intcode(const auto& program, bool debug=false)
    : m_program(program),
      m_debug(debug)
  { }

  enum class State {
    ok,
    waiting_for_input,
    waiting_for_output,
    finished,
    error,
  };

  struct Opcode {
    Opcode(int full_state) 
    {
      op = full_state % 100;
      a  = (full_state /   100) % 10;
      b  = (full_state /  1000) % 10;
      c  = (full_state / 10000) % 10;
    }

    int op, a, b, c;
  };

  State step()
  {
    const Opcode opcode{m_program.at(m_pos)};
    if (m_debug) {
      //std::printf("Opcode %4d -> c:%d, b:%d, a:%d, op:%02d\n", m_program.at(m_pos), opcode.c, opcode.b, opcode.a, opcode.op);
    }
    switch (opcode.op) {
      /// A + B -> C
      case 1: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        if (m_debug) {
          std::cout << "+ " << imm(m_pos);
          std::cout << ", " << imm(m_pos+1);
          if (not opcode.a) std::cout << "(" << pos(m_pos+1) << ")";
          std::cout << ", " << imm(m_pos+2);
          if (not opcode.b) std::cout << "(" << pos(m_pos+2) << ")";
          std::cout << ", " << imm(m_pos+3);
          std::cout << std::endl;
          std::cout << "  " << pos(m_pos+3) << " := "
                    << a << "+" << b << "(=" << a+b << ")"
                    << std::endl
                    << std::endl;
        }
        auto& target = pos(m_pos+3);
        target = a + b;
        m_pos += 4;
        break;
      }
      /// A * B -> C
      case 2: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        if (m_debug) {
          std::cout << "* " << imm(m_pos);
          std::cout << ", " << imm(m_pos+1);
          if (not opcode.a) std::cout << "(" << pos(m_pos+1) << ")";
          std::cout << ", " << imm(m_pos+2);
          if (not opcode.b) std::cout << "(" << pos(m_pos+2) << ")";
          std::cout << ", " << imm(m_pos+3);
          std::cout << std::endl;
          std::cout << "  " << pos(m_pos+3) << " := "
                    << a << "*" << b << "(=" << a*b << ")"
                    << std::endl
                    << std::endl;
        }
        auto& target = pos(m_pos+3);
        target = a * b;
        m_pos += 4;
        break;
      }
      /// input -> A
      case 3: {
        auto res = getInput(pos(m_pos + 1));
        if (m_debug) {
          std::cout << "> " << pos(m_pos + 1)
                    << " -> " << imm(m_pos + 1);
          std::cout << std::endl
                    << std::endl;
        }
        if (res != State::ok) {
          return res;
        }
        m_pos += 2;
        break;
      }
      /// output <- A
      case 4: {
        if (m_debug) {
          std::cout << "< " << imm(m_pos);
          std::cout << ", " << imm(m_pos+1);
          if (not opcode.a) std::cout << "(" << pos(m_pos+1) << ")";
          std::cout << std::endl
                    << std::endl;
        }
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto res = setOutput(a);
        if (res != State::ok) {
          return res;
        }
        m_pos += 2;
        break;
      }
      /// Jump if true
      case 5: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        if (a != 0) {
          m_pos = b;
        } else {
          m_pos += 3;
        }
        break;
      }
      /// Jump if false
      case 6: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        if (a == 0) {
          m_pos = b;
        } else {
          m_pos += 3;
        }
        break;
      }
      /// less than
      case 7: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        auto& target = pos(m_pos+3);
        if (a < b) {
          target = 1;
        } else {
          target = 0;
        }
        m_pos += 4;
        break;
      }
      /// equals
      case 8: {
        auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        auto& b = opcode.b ? imm(m_pos+2) : pos(m_pos+2);
        auto& target = pos(m_pos+3);
        if (a == b) {
          target = 1;
        } else {
          target = 0;
        }
        m_pos += 4;
        break;
      }
      /// Stop
      case 99: {
        return State::finished;
      }
      default: {
        throw std::runtime_error("Unknown opcode");
      }
    }
    return State::ok;
  }

  State run()
  {
    for (;;) {
      if (m_pos >= m_program.size()) { 
        return State::error;
      }
      if (auto res = step(); res != State::ok) {
        return res;
      }
    }
  }

  int getResult()
  {
    return m_program.at(0);
  }

  void setInput(int value)
  {
    m_inputs.push(value);
  }

  bool getOutput(int& value)
  {
    if (m_outputs.size() == 0) {
      return false;
    }
    auto res = m_outputs.front();
    m_outputs.pop();
    value = res;
    return true;
  }

private:

  int& pos(int index)
  {
    return m_program.at(m_program.at(index));
  }

  int& imm(int index)
  {
    return m_program.at(index);
  }

  State getInput(int& value)
  {
    if (m_inputs.size() == 0) {
      return State::waiting_for_input;
    }
    value = m_inputs.front();
    m_inputs.pop();
    return State::ok;
  }

  State setOutput(int value)
  {
    m_outputs.push(value);
    return State::ok;
  }


  unsigned int m_pos{0};
  std::vector<int> m_program;
  std::queue<int> m_inputs, m_outputs;
  bool m_debug;
};

