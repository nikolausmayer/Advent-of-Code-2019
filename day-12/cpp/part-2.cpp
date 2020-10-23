
#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <map>
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


  std::map<std::array<int, 8>, unsigned> x_states, y_states, z_states;

  unsigned x_period{0};
  unsigned y_period{0};
  unsigned z_period{0};

  for (unsigned round{0};; ++round) {
    std::array<int, 8> X{bodies[0].x, bodies[0].vx,
                         bodies[1].x, bodies[1].vx,
                         bodies[2].x, bodies[2].vx,
                         bodies[3].x, bodies[3].vx};
    std::array<int, 8> Y{bodies[0].y, bodies[0].vy,
                         bodies[1].y, bodies[1].vy,
                         bodies[2].y, bodies[2].vy,
                         bodies[3].y, bodies[3].vy};
    std::array<int, 8> Z{bodies[0].z, bodies[0].vz,
                         bodies[1].z, bodies[1].vz,
                         bodies[2].z, bodies[2].vz,
                         bodies[3].z, bodies[3].vz};

    if (x_period == 0 and x_states.find(X) != x_states.end()) {
      x_period = round - x_states[X];
    }
    if (y_period == 0 and y_states.find(Y) != y_states.end()) {
      y_period = round - y_states[Y];
    }
    if (z_period == 0 and z_states.find(Z) != z_states.end()) {
      z_period = round - z_states[Z];
    }
    if (x_period != 0 and
        y_period != 0 and
        z_period != 0) {
      /// Get prime factors
      auto is_prime = [](unsigned n) {
        if (n <= 2)
          return true;
        if (n%2 == 0)
          return false;
        for (auto i{3}; i*i <= n; i+=2) {
          if (n % i == 0)
            return false;
        }
        return true;
      };
      std::set<unsigned> all_prime_factors;
      std::map<unsigned, unsigned> prime_factors_x;
      for (auto i{2}; x_period > 1; ++i) {
        while (is_prime(i) and x_period % i == 0) {
          x_period /= i;
          ++prime_factors_x[i];
          all_prime_factors.insert(i);
        }
      }
      std::map<unsigned, unsigned> prime_factors_y;
      for (auto i{2}; y_period > 1; ++i) {
        while (is_prime(i) and y_period % i == 0) {
          y_period /= i;
          ++prime_factors_y[i];
          all_prime_factors.insert(i);
        }
      }
      std::map<unsigned, unsigned> prime_factors_z;
      for (auto i{2}; z_period > 1; ++i) {
        while (is_prime(i) and z_period % i == 0) {
          z_period /= i;
          ++prime_factors_z[i];
          all_prime_factors.insert(i);
        }
      }

      /// Least common multiple
      unsigned long long total{1};
      for (const auto pf : all_prime_factors) {
        auto max_power = std::max({prime_factors_x[pf],
                                   prime_factors_y[pf],
                                   prime_factors_z[pf]});
        for (auto i{1}; i <= max_power; ++i)
          total *= pf;
      }
      std::cout << total << std::endl;
      
      return EXIT_SUCCESS;
    }

    x_states[X] = round;
    y_states[Y] = round;
    z_states[Z] = round;

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


}

