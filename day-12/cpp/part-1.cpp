
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

class Body
{
  public:
  Body(int x, int y, int z)
    : x(x), y(y), z(z)
  { }

  void getInfluence(const Body& other)
  {
    if      (other.x < x) --vx;
    else if (other.x > x) ++vx;

    if      (other.y < y) --vy;
    else if (other.y > y) ++vy;

    if      (other.z < z) --vz;
    else if (other.z > z) ++vz;
  }

  void updatePosition()
  {
    x += vx;
    y += vy;
    z += vz;
  }

  int getEnergy() const
  {
    return (std::abs(x)  + std::abs(y)  + std::abs(z)) *
           (std::abs(vx) + std::abs(vy) + std::abs(vz));
  }

  friend std::ostream& operator<<(std::ostream& stream,
                                  const Body& body)
  {
    stream << "pos=<x=" << std::setw(3) << body.x
           << ", y=" << std::setw(3) << body.y
           << ", z=" << std::setw(3) << body.z
           << ">, vel=<x=" << std::setw(3) << body.vx
           << ", y=" << std::setw(3) << body.vy
           << ", z=" << std::setw(3) << body.vz
           << ">";
    return stream;
  }

  private:
  int x, y, z;
  int vx{0}, vy{0}, vz{0};
};

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::vector<Body> bodies;

  std::string line;
  while (std::getline(infile, line)) {
    int x, y, z;
    std::sscanf(line.data(), "<x=%d, y=%d, z=%d>\n", &x, &y, &z);
    bodies.emplace_back(x, y, z);
  }

  for (auto i{0}; i < 1000; ++i) {

    for (std::size_t a{0}; a < bodies.size(); ++a) {
      for (std::size_t b{0}; b < bodies.size(); ++b) {
        if (a == b) continue;
        bodies[a].getInfluence(bodies[b]);
      }
    }
    for (auto& body : bodies) {
      body.updatePosition();
    }
  }
  auto total_energy{0};
  for (const auto& body: bodies) {
    total_energy += body.getEnergy();
  }
  std::cout << "Sum of total energy: " << total_energy
            << std::endl;
}

