
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

class Line
{
  public:

  void add_step(char dir, int count)
  {
    m_line.push_back(std::make_pair(dir, count));
  }

  void run() 
  {
    int x{0}, y{0};
    //m_visited_locations.emplace(std::make_pair(x, y));
    for (const auto& [dir, count] : m_line) {
      int dx = (dir == 'L' ? -1 : 
                dir == 'R' ?  1 : 0);
      int dy = (dir == 'U' ? -1 : 
                dir == 'D' ?  1 : 0);
      for (int idx = 0; idx < count; ++idx) {
        x += dx;
        y += dy;
        m_visited_locations.emplace_back(std::make_pair(x, y));
      }
    }
  }

  std::vector<std::pair<int, int>> get_intersections(const Line&other)
  {
    std::vector<std::pair<int, int>> results;
    for (const auto& pos : m_visited_locations) {
      auto search = std::find(other.m_visited_locations.begin(),
                              other.m_visited_locations.end(),
                              pos);
      if (search != other.m_visited_locations.end()) {
        results.push_back(pos);
      }
    }
    return results;
  }

  std::vector<std::pair<char, int>> m_line;
  std::vector<std::pair<int, int>> m_visited_locations;
};



int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Cannot read input file");
  }

  char dir{'0'};
  int number{0};
  std::vector<Line> lines;
  Line current_line;
  for (;;) {
    char current = infile.get();
    if (infile.eof()) {
      current_line.add_step(dir, number);
      current_line.run();
      lines.emplace_back(std::move(current_line));
      break;
    }
    switch (current) {
      case '\n':
        current_line.add_step(dir, number);
        current_line.run();
        lines.emplace_back(std::move(current_line));
        current_line = Line();
        continue;
      case 'U':
      case 'D':
      case 'L':
      case 'R':
        dir = current;
        number = 0;
        break;
      case ',':
        current_line.add_step(dir, number);
        break;
      default:
        number = number*10 + (current - 48);
    }
  }

  auto isects{lines.at(0).get_intersections(lines.at(1))};
  auto min_signal_delay{-1};
  for (const auto& pos : isects) {
    const auto& la{lines.at(0).m_visited_locations};
    const auto& lb{lines.at(1).m_visited_locations};
    /// "+1" because (0,0) should be a step here
    auto a{std::find(la.begin(), la.end(), pos) - la.begin() + 1};
    auto b{std::find(lb.begin(), lb.end(), pos) - lb.begin() + 1};
    if (min_signal_delay < 0 or (a + b) < min_signal_delay) {
      min_signal_delay = a + b;
    }
  }
  std::cout << "Signal delay from central port to closest intersection: " << min_signal_delay << std::endl;

  return EXIT_SUCCESS;
}

