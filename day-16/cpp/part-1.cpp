
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::string rawline;
  infile >> rawline;

  std::vector<int> line;
  for (const char c : rawline) {
    line.push_back(atoi(&c));
  }

  const std::vector<int> pattern{0, 1, 0, -1};

  for (auto repetition{0}; repetition < 100; ++repetition) {
    std::vector<int> out_line;
    out_line.reserve(line.size());
    for (auto out_idx{0u}; out_idx < line.size(); ++out_idx) {
      std::vector<int> tmp_pattern;
      tmp_pattern.reserve(line.size() * 2);
      for (auto pat_idx{0u};; pat_idx = (pat_idx+1)%4) {
        for (auto rep{0u}; rep <= out_idx; ++rep)
          tmp_pattern.push_back(pattern[pat_idx]);
        if (tmp_pattern.size() > line.size())
          break;
      }

      long sum{0};
      for (auto idx{0u}; idx < line.size(); ++idx)
        sum += line[idx] * tmp_pattern[idx+1];

      out_line.push_back(std::abs(sum) % 10);
    }
    line = out_line;
  }
  for (auto i{0u}; i < 8; ++i) std::cout << line[i];
  std::cout << std::endl;
}

