#include <cstdint>
#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;
  
  std::array<bool,25> map;
  {
    std::size_t idx = 0;
    while (true) {
      std::string line;
      std::getline(infile, line);
      if (infile.eof())
        break;
      for (auto c : line)
        map[idx++] = (c == '#');
    }
  }

  //auto print = [&map]() {
  //  for (auto y = 0; y < 5; y++) {
  //    for (auto x = 0; x < 5; x++) {
  //      std::cout << ".#"[map[y*5+x]];
  //    }
  //    std::cout << std::endl;
  //  }
  //};

  auto update = [&map]() {
    static const std::vector<std::pair<int,int>> dxdy = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    auto newmap = map;
    for (auto y = 0; y < 5; y++) {
      for (auto x = 0; x < 5; x++) {
        auto sum = 0;
        for (auto [dx,dy] : dxdy)
          if (x+dx >= 0 and x+dx < 5 and
              y+dy >= 0 and y+dy < 5)
            sum += map[(y+dy)*5+x+dx];
        if (map[y*5+x] and sum != 1)
          newmap[y*5+x] = false;
        else if (not map[y*5+x] and (sum == 1 or sum == 2))
          newmap[y*5+x] = true;
        else 
          newmap[y*5+x] = map[y*5+x];
      }
    }
    map = newmap;
  };

  std::set<decltype(map)> visited;

  while (true) {
    update();
    if (visited.contains(map)) {
      //print();
      auto biodiversity = 0;
      for (auto i = 0, p = 1; i < 25; i++, p *= 2)
        biodiversity += map[i] * p;
      std::cout << biodiversity << std::endl;
      break;
    }
    visited.insert(map);
  }

  return EXIT_SUCCESS;
}

