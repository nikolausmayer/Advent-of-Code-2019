#include <vector>
#include <stdexcept>

class Intcode
{
  public:
  Intcode(const auto& program)
    : m_program(program)
  { }

  bool step()
  {
    const auto opcode{m_program.at(m_pos)};
    switch (opcode) {
      case 1:
        m_program.at(m_program.at(m_pos + 3)) = m_program.at(m_program.at(m_pos + 1)) + m_program.at(m_program.at(m_pos + 2));
        m_pos += 4;
        break;
      case 2:
        m_program.at(m_program.at(m_pos + 3)) = m_program.at(m_program.at(m_pos + 1)) * m_program.at(m_program.at(m_pos + 2));
        m_pos += 4;
        break;
      case 99:
        //std::cout << m_program.at(0) << std::endl;
        return false;
      default:
        throw std::runtime_error("Bad opcode");
    }
    return true;
  }

  void run()
  {
    for (;;) {
      if (m_pos >= m_program.size()) { break; }
      if (not step()) { break; }
    }
  }

  int result()
  {
    return m_program.at(0);
  }

  private:
  unsigned int m_pos{0};
  std::vector<int> m_program;
};

