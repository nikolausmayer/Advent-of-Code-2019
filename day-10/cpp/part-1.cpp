
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

  int max_asteroids_seen{0};
  int best_base_x, best_base_y;

  for (int base_y{0}; base_y < H; ++base_y) {
    for (int base_x{0}; base_x < W; ++base_x) {

      if (not map[base_y][base_x])
        continue;

      //std::cout << "Testing base at (" << base_x << "," << base_y << ")" << std::endl;

      int asteroids_seen{0};

      for (int ay{0}; ay < H; ++ay) {
        for (int ax{0}; ax < W; ++ax) {
          if (not map[ay][ax] or 
              (ax == base_x and ay == base_y))
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
              //std::cout << "    Testing (" << test_x << "," << test_y << ")" << std::endl;
              if (map[test_y][test_x]) {
                view_is_blocked = true;
                //std::cout << "  Asteroid (" << ax << "," << ay << ") is blocked by (" << test_x << "," << test_y << ")" << std::endl;
                break;
              }
            }
          }
          if (not view_is_blocked) {
            ++asteroids_seen;
            //std::cout << "  Asteroid (" << ax << "," << ay << ") is visible." << std::endl;
          }
        }
      }

      //std::cout << "  This base sees " << asteroids_seen << " asteroids." << std::endl << std::endl;
      if (asteroids_seen > max_asteroids_seen) {
        max_asteroids_seen = asteroids_seen;
        best_base_x = base_x;
        best_base_y = base_y;
      }
    }
  }

  std::cout << "Maximum number of asteroids seen: " 
            << max_asteroids_seen << ", at base ("
            << best_base_x << "," << best_base_y << ")."
            << std::endl;
}

