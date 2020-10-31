
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

  auto offset{0u};
  for (auto i{0u}; i < 7; ++i)
    offset = 10*offset + line[i];

  std::vector<int> bigline;
  bigline.reserve(line.size() * 10'000);
  for (auto i{0u}; i < 10'000; ++i)
    for (auto v : line)
      bigline.push_back(v);
  line = bigline;

  const std::vector<int> pattern{0, 1, 0, -1};

  int *Aline = new int[line.size()];
  for (unsigned i{0}; i < line.size(); ++i)
    Aline[i] = line[i];
  int *Aout_line = new int[line.size()];

  for (auto repetition{0}; repetition < 100; ++repetition) {

    Aout_line[line.size()-1] = Aline[line.size()-1];
    for (unsigned idx{line.size()-2};; --idx) {
      Aout_line[idx] = Aout_line[idx+1] + Aline[idx];
      if (idx == 0) break;
    }

    for (unsigned idx{0}; idx < line.size(); ++idx) {
      for (unsigned udx{2*idx+1}, step{0}; 
           udx < line.size(); 
           udx += (idx+1), ++step) {
        auto factor = ((step / 2) % 2) ? 1: -1;
        Aout_line[idx] += factor * Aout_line[udx];
      }
    }

    for (unsigned long idx{0}; idx < line.size(); ++idx) {
      Aout_line[idx] = std::abs(Aout_line[idx] % 10);
    }

    std::swap(Aline, Aout_line);
  }

  for (auto i{0u}; i < 8; ++i) std::cout << Aline[i+offset];
  std::cout << std::endl;

  delete[] Aline;
  delete[] Aout_line;
}

