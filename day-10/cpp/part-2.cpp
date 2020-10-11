
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include <tuple>
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
  
  std::vector<std::vector<bool>> map{{}};
  for (;;) {
    char c;
    infile.get(c);
    if (infile.eof())
      break;
    switch (c) {
      case '.': map.back().push_back(false); break;
      case '#': map.back().push_back(true); break;
      case '\n': map.emplace_back(std::vector<bool>{}); break;
    }
  }
  map.pop_back();

  const int H{static_cast<int>(map.size())};
  const int W{static_cast<int>(map.back().size())};

  const int base_x{26};
  const int base_y{36};

  std::list<std::tuple<float,int,int,bool>> asteroids_by_angle;
  for (int y{0}; y < H; ++y) {
    for (int x{0}; x < W; ++x) {
      if (not map[y][x] or (x == base_x and y == base_y))
        continue;

      const float angle{std::atan2(-(base_y - y), 
                                     base_x - x)};
      asteroids_by_angle.emplace_back(-angle, x, y, false);
    }
  }
  asteroids_by_angle.sort([](auto a, auto b) {
              return std::get<0>(a) < std::get<0>(b);
            });
  while (std::get<0>(asteroids_by_angle.front()) < M_PI/2.) {
    asteroids_by_angle.emplace_back(asteroids_by_angle.front());
    asteroids_by_angle.pop_front();
  }

  auto asteroids_lasered{0};
  while (std::any_of(asteroids_by_angle.begin(),
                     asteroids_by_angle.end(),
                     [](const auto& elem){
                       return not std::get<3>(elem);
                     })) {
    auto last_lasered_angle{9999.};
    for (auto& [angle, ax, ay, lasered] : asteroids_by_angle) {
      if (lasered)
        continue;
      if (std::abs(angle - last_lasered_angle) < 1e-6)
        continue;

      const int dx{ax - base_x};
      const int dy{ay - base_y};
      const int max_divisor{std::max(std::abs(dx),
                                     std::abs(dy))};

      bool view_is_blocked{false};
      for (int div{1}; 
           div <= max_divisor and not view_is_blocked; 
           ++div) {
        if (dx % div or dy % div)
          continue;
        const int max_step{div};
        for (int step{1}; step < max_step; ++step) {
          const int test_x{base_x + step * (dx / div)};
          const int test_y{base_y + step * (dy / div)};
          if (test_x < 0 or test_x >= W or
              test_y < 0 or test_y >= H)
            continue;
          if (map[test_y][test_x]) {
            view_is_blocked = true;
            //std::cout << "(" << ax << "," << ay 
            //          << ") blocked by (" << test_x
            //          << "," << test_y << ")" << std::endl;
            break;
          }
        }
      }

      if (view_is_blocked)
        continue;

      ++asteroids_lasered;
      //std::cout << "Lasering asteroid #" << asteroids_lasered
      //          << " (" << ax << "," << ay << ")" << std::endl;
      lasered = true;
      map[ay][ax] = false;
      last_lasered_angle = angle;
      last_lasered_angle = angle;

      if (asteroids_lasered == 200) {
        std::cout << "200th vaporized asteroid is ("
                  << ax << "," << ay << ")" << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

}

