#include <cassert>
#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <vector>
#include <stdexcept>

class Intcode
{
public:

  typedef long long DATA_t;

  Intcode(const auto& program)
    : m_program(program)
  { }

  enum class State {
    ok,
    waiting_for_input,
    waiting_for_output,
    finished,
    error,
  };

  struct Opcode {
    Opcode(DATA_t full_state) 
    {
      op = full_state % 100;
      a  = (full_state /   100) % 10;
      b  = (full_state /  1000) % 10;
      c  = (full_state / 10000) % 10;
    }

    int op, a, b, c;
  };

  DATA_t& getCode(int pos) 
  {
    if (pos >= 0 and pos < (int)m_program.size()) {
      return m_program[pos];
    } else {
      return m_program_extra_space[pos];
    }
  }


  DATA_t& getParam(DATA_t code, DATA_t ptr) 
  {
    switch (code) {
      case 0: return pos(ptr);
      case 1: return imm(ptr);
      case 2: return getCode(getCode(ptr) + m_relative_base);
      default: throw std::runtime_error("Bad code");
    }
  }


  State step()
  {
    const Opcode opcode{getCode(m_pos)};
    switch (opcode.op) {
      /// A + B -> C
      case 1: {
        //auto& a = opcode.a ? imm(m_pos+1) : pos(m_pos+1);
        const auto& a = getParam(opcode.a, m_pos+1);
        const auto& b = getParam(opcode.b, m_pos+2);
        auto& target = getParam(opcode.c, m_pos+3);
        const auto tmp = a + b;
        assert(tmp - a == b);
        assert(tmp - b == a);
        target = tmp;
        m_pos += 4;
        break;
      }
      /// A * B -> C
      case 2: {
        const auto& a = getParam(opcode.a, m_pos+1);
        const auto& b = getParam(opcode.b, m_pos+2);
        auto& target = getParam(opcode.c, m_pos+3);
        const auto tmp = a * b;
        assert(a == 0 or tmp / a == b);
        assert(b == 0 or tmp / b == a);
        target = tmp;
        m_pos += 4;
        break;
      }
      /// input -> A
      case 3: {
        DATA_t& write_input_to = (opcode.a == 0)
          ? pos(m_pos + 1)
          : imm(imm(m_pos + 1) + m_relative_base);
        auto res = getInput(write_input_to);
        if (res != State::ok) {
          return res;
        }
        m_pos += 2;
        break;
      }
      /// output <- A
      case 4: {
        auto& a = getParam(opcode.a, m_pos+1);
        auto res = setOutput(a);
        if (res != State::ok) {
          return res;
        }
        m_pos += 2;
        break;
      }
      /// Jump if true
      case 5: {
        auto& a = getParam(opcode.a, m_pos+1);
        auto& b = getParam(opcode.b, m_pos+2);
        if (a != 0) {
          m_pos = b;
        } else {
          m_pos += 3;
        }
        break;
      }
      /// Jump if false
      case 6: {
        auto& a = getParam(opcode.a, m_pos+1);
        auto& b = getParam(opcode.b, m_pos+2);
        if (a == 0) {
          m_pos = b;
        } else {
          m_pos += 3;
        }
        break;
      }
      /// less than
      case 7: {
        auto& a = getParam(opcode.a, m_pos+1);
        auto& b = getParam(opcode.b, m_pos+2);
        auto& target = getParam(opcode.c, m_pos+3);
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
        auto& a = getParam(opcode.a, m_pos+1);
        auto& b = getParam(opcode.b, m_pos+2);
        auto& target = getParam(opcode.c, m_pos+3);
        if (a == b) {
          target = 1;
        } else {
          target = 0;
        }
        m_pos += 4;
        break;
      }
      /// set relative base
      case 9: {
        auto& a{getParam(opcode.a, m_pos+1)};
        m_relative_base += a;
        m_pos += 2;
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
      if (auto res = step(); res != State::ok) {
        return res;
      }
    }
  }

  DATA_t getResult()
  {
    return getCode(0);
  }

  void setInput(DATA_t value)
  {
    m_inputs.push(value);
  }

  bool getOutput(DATA_t& value)
  {
    if (m_outputs.size() == 0) {
      return false;
    }
    auto res = m_outputs.front();
    m_outputs.pop();
    value = res;
    return true;
  }

  DATA_t getLastOutput()
  {
    return m_last_output;
  }

  void sendOutputTo(Intcode* recipient)
  {
    m_output_recipient = recipient;
  }

  void getInputFrom(Intcode* provider)
  {
    m_input_provider = provider;
  }

private:

  DATA_t& pos(int index)
  {
    return getCode(getCode(index));
  }

  DATA_t& imm(int index)
  {
    return getCode(index);
  }

  State getInput(DATA_t& value)
  {
    if (m_inputs.size() == 0) {
      if (idle_count++ >= 10)
        return State::waiting_for_input;
      setInput(-1);
      //if (m_input_provider) {
      //  std::unique_lock<std::mutex> lock(m_input_available_mutex);
      //  m_input_available.wait(lock, [this]{return (m_inputs.size() != 0);});
      //} else {
      //  return State::waiting_for_input;
      //}
    }
    value = m_inputs.front();
    m_inputs.pop();
    return State::ok;
  }

  State setOutput(DATA_t value)
  {
    idle_count = 0;

    if (m_output_recipient) {
      {
        std::lock_guard<std::mutex> lock(m_output_recipient->m_input_available_mutex);
        m_output_recipient->m_inputs.push(value);
      }
      m_output_recipient->m_input_available.notify_one();
    } else {
      m_outputs.push(value);
    }

    m_last_output = value;

    return State::ok;
  }


  int idle_count{0};
  DATA_t m_pos{0};
  std::vector<DATA_t> m_program;
  std::map<DATA_t, DATA_t> m_program_extra_space;
  std::queue<DATA_t> m_inputs, m_outputs;
  DATA_t m_last_output{-1};
  DATA_t m_relative_base{0};

  Intcode *m_output_recipient{nullptr}, *m_input_provider{nullptr};
  std::condition_variable m_input_available;
  std::mutex m_input_available_mutex;
};

