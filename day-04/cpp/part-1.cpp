
#include <array>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  int lower, upper;
  infile >> lower;
  infile.get();
  infile >> upper;

  const auto check = [](int n){
    static std::array<char, 6> chars;
    std::to_chars(chars.data(), chars.data() + chars.size(), n);
    auto double_found{false};
    for (auto i = 0; i < 5; ++i) {
      if (chars[i] > chars[i+1]) { return false; }
      if (chars[i] == chars[i+1]) { double_found = true; }
    }
    return double_found;
  };

  auto valid_candidates{0};
  for (int candidate = lower; candidate <= upper; ++candidate) {
    if (check(candidate)){
      ++valid_candidates;
    }
  }
  std::cout << valid_candidates << std::endl;

  return EXIT_SUCCESS;
}

