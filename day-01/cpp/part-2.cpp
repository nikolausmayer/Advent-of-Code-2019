#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("bad arguments");
  }
  auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("cannot open file");
  }

  auto required_total_fuel{0};
  while (not infile.eof()) {
    std::string module_mass;
    infile >> module_mass;
    if (infile.eof()) { break; }
    const auto mass = std::stoi(module_mass);

    const auto step = [](auto mass) { return mass / 3 - 2; };
    auto required_fuel{mass};
    for (;;) {
      required_fuel = step(required_fuel);
      if (required_fuel <= 0) {
        break;
      }
      required_total_fuel += required_fuel;
    }
  }
  
  std::cout << "Total required fuel: " 
            << required_total_fuel << std::endl;

  return EXIT_SUCCESS;
}

