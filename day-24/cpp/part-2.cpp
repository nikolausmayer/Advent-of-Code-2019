#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;
  
  std::map<std::tuple<int,int,int>,bool> map;
  {
    int y = -2;
    while (true) {
      std::string line;
      std::getline(infile, line);
      if (infile.eof())
        break;
      for (auto x = -2; x <= 2; x++)
        map[{x,y,0}] = (line[x+2] == '#');
      y++;
    }
  }
  map.erase(map.find({0,0,0}));

  std::map<std::tuple<int,int>, 
           std::vector<std::tuple<int,int,int>>> neighbors;
  for (auto y = -2; y <= 2; y++) {
    for (auto x = -2; x <=2 ; x++) {
      if (x == 0 and y == 0)
        continue;
      
      auto& list = neighbors[{x,y}];
      
      if (y == -2) list.push_back({ 0,-1,-1});
      if (y ==  2) list.push_back({ 0, 1,-1});
      if (x == -2) list.push_back({-1, 0,-1});
      if (x ==  2) list.push_back({ 1, 0,-1});
      
      if (y  > -2 and (x !=  0 or y !=  1))
        list.push_back({ x, y-1, 0});
      if (x  > -2 and (x !=  1 or y !=  0))
        list.push_back({ x-1, y, 0});
      if (y  <  2 and (x !=  0 or y != -1))
        list.push_back({ x, y+1, 0});
      if (x  <  2 and (x != -1 or y !=  0))
        list.push_back({ x+1, y, 0});
    }
  }
  for (auto i = -2; i <= 2; i++) {
    neighbors[{0,-1}].push_back({i,-2,1});
    neighbors[{0, 1}].push_back({i, 2,1});
    neighbors[{-1,0}].push_back({-2,i,1});
    neighbors[{ 1,0}].push_back({ 2,i,1});
  }

  //for (const auto& [xy,n] : neighbors) {
  //  auto [x,y] = xy;
  //  std::cout << "(" << x << "," << y << "):\n";
  //  for (auto [x,y,dz] : n) {
  //    std::cout << "  (" << x << "," << y << "," << (dz > 0 ? '+' : ' ') << dz << ")\n";
  //  }
  //}

  for (auto round = 0; round < 200; round++) {
    auto newmap = map;
    for (const auto& [xyz,b] : map) {
      if (not b)
        continue;
      const auto [x,y,z] = xyz;
      for (auto [nx,ny,ndz] : neighbors[{x,y}])
        if (not newmap.contains({nx,ny,z+ndz}))
          newmap[{nx,ny,z+ndz}] = false;
    }
    map = newmap;

    for (const auto& [xyz,b] : map) {
      const auto [x,y,z] = xyz;
      const auto m = map[{x,y,z}];
      auto sum = 0;
      for (auto [nx,ny,ndz] : neighbors[{x,y}])
        sum += map[{nx,ny,z+ndz}];
      if (m and sum != 1)
        newmap[{x,y,z}] = false;
      else if (not m and (sum == 1 or sum == 2))
        newmap[{x,y,z}] = true;
    }

    map = newmap;
  }

  //{
  //  auto minz = 0;
  //  auto maxz = 0;
  //  for (const auto& [xyz,b] : map) {
  //    if (b) {
  //      minz = std::min(minz, std::get<2>(xyz));
  //      maxz = std::max(maxz, std::get<2>(xyz));
  //    }
  //  }
  //  for (auto z = minz; z <= maxz; z++) {
  //    std::cout << "Depth " << z << ":\n";
  //    for (auto y = -2; y <= 2; y++) {
  //      for (auto x = -2; x <= 2; x++) {
  //        std::cout << ((x == 0 and y == 0) 
  //                      ? '?' 
  //                      : ".#"[map[{x,y,z}]]);
  //      }
  //      std::cout << std::endl;
  //    }
  //    std::cout << std::endl;
  //  }
  //}


  auto bugs = 0;
  for (const auto& [_,b] : map)
    bugs += b;
  
  std::cout << bugs << std::endl;

  return EXIT_SUCCESS;
}

