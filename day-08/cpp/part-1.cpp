
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

  const std::string encoded = [&infile]{
    std::string tmp;
    infile >> tmp;
    return tmp;
  }();

  const auto W{25};
  const auto H{6};
  const auto length{encoded.size()};

  auto fewest_zeroes{-1};
  auto ones_times_twos{0};
  for (unsigned long layer_start{0}; layer_start < length; layer_start += W*H) {
    auto zeroes{0};
    auto ones{0};
    auto twos{0};
    for (unsigned long idx{layer_start}; idx < layer_start+W*H; ++idx) {
      switch(encoded.data()[idx]) {
        case '0': { ++zeroes; break; }
        case '1': { ++ones; break; }
        case '2': { ++twos; break; }
      }
    }
    if (zeroes < fewest_zeroes or fewest_zeroes == -1) {
      fewest_zeroes = zeroes;
      ones_times_twos = ones * twos;
    }
  }

  std::cout << "Result: " << ones_times_twos << std::endl;
}

