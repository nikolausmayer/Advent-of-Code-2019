#include <cstdint>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
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
  
  std::map<std::array<int, 2>, int> map;
  std::map<std::array<char,2>, int> portals;
  auto H = 0;
  auto W = 0;
  {
    /// Get whole input
    std::vector<std::string> rawlines;
    while (true) {
      std::string line;
      std::getline(infile, line);
      if (infile.eof())
        break;
      rawlines.push_back(std::move(line));
    }
    /// Transcribe donut shape
    H = (int)rawlines.size();
    W = (int)rawlines[0].size();
    for (auto y = 2; y < H-2; y++)
      for (auto x = 2; x < W-2; x++)
        if (rawlines[y][x] == '#')
          map[{x,y}] = 1;
        else if (rawlines[y][x] == '.')
          map[{x,y}] = 2;
    /// Find hole size
    auto hole_l = 0;
    auto hole_r = 0;
    for (auto d = 1; d < W; d++) {
      if (W/2-d >= 0) {
        auto l = rawlines[H/2][W/2-d];
        if (hole_l == 0 and (l == '.' or l == '#'))
          hole_l = W/2-d;
      }
      if (W/2+d < W) {
        auto r = rawlines[H/2][W/2+d];
        if (hole_r == 0 and (r == '.' or r == '#'))
          hole_r = W/2+d;
      }
    }
    auto hole_t = 0;
    auto hole_b = 0;
    for (auto d = 1; d < H; d++) {
      if (H/2-d >= 0) {
        auto t = rawlines[H/2-d][W/2];
        if (hole_t == 0 and (t == '.' or t == '#'))
          hole_t = H/2-d;
      }
      if (H/2+d < H) {
        auto b = rawlines[H/2+d][W/2];
        if (hole_b == 0 and (b == '.' or b == '#'))
          hole_b = H/2+d;
      }
    }
    /// Find portals (outside)
    for (auto x = 2; x < W-2; x++) {
      if (char c = rawlines[0][x]; c != ' ') {
        std::array<char, 2> portal{c, rawlines[1][x]};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{x,2}] = portals[portal];
      }
      if (char c = rawlines[H-1][x]; c != ' ') {
        std::array<char, 2> portal{rawlines[H-2][x], c};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{x,H-3}] = portals[portal];
      }
    }
    for (auto y = 2; y < H-2; y++) {
      if (char c = rawlines[y][0]; c != ' ') {
        std::array<char, 2> portal{c, rawlines[y][1]};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{2,y}] = portals[portal];
      }
      if (char c = rawlines[y][W-1]; c != ' ') {
        std::array<char, 2> portal{rawlines[y][W-2], c};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{W-3,y}] = portals[portal];
      }
    }
    /// Find portals (inside)
    for (auto x = hole_l+1; x < hole_r; x++) {
      if (char c = rawlines[hole_t+1][x]; c != ' ') {
        if (rawlines[hole_t+2][x] == ' ')
          continue;
        std::array<char, 2> portal{c, rawlines[hole_t+2][x]};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{x,hole_t}] = portals[portal];
      }
      if (char c = rawlines[hole_b-1][x]; c != ' ') {
        if (rawlines[hole_b-2][x] == ' ')
          continue;
        std::array<char, 2> portal{rawlines[hole_b-2][x], c};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{x,hole_b}] = portals[portal];
      }
    }
    for (auto y = hole_t+1; y < hole_b; y++) {
      if (char c = rawlines[y][hole_l+1]; c != ' ') {
        if (rawlines[y][hole_l+2] == ' ')
          continue;
        std::array<char, 2> portal{c, rawlines[y][hole_l+2]};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{hole_l,y}] = portals[portal];
      }
      if (char c = rawlines[y][hole_r-1]; c != ' ') {
        if (rawlines[y][hole_r-2] == ' ')
          continue;
        std::array<char, 2> portal{rawlines[y][hole_r-2], c};
        if (not portals.contains(portal))
          portals[portal] = portals.size()+3;
        map[{hole_r,y}] = portals[portal];
      }
    }
  }

  //for (auto y = 2; y < H-2; y++) {
  //  for (auto x = 2; x < W-2; x++)
  //    switch (map[{x,y}]) {
  //      case 0:  std::cout << ' '; break;
  //      case 1:  std::cout << '#'; break;
  //      case 2:  std::cout << '.'; break;
  //      default: std::cout << (char)('A' + map[{x,y}] - 3);
  //    }
  //  std::cout << std::endl;
  //}

  /// Backwards Dijkstra 
  std::map<std::array<int, 2>, int> oracle;
  const auto dxdy = std::vector<std::array<int,2>>{{0,-1},{1,0},{0,1},{-1,0}};
  {
    std::set<std::array<int, 2>> visited;
    auto cmp = [&](const auto& a, const auto& b) {
      return a[0] > b[0];
    };
    std::priority_queue<std::array<int,3>,
                        std::vector<std::array<int,3>>,
                        decltype(cmp)> queue(cmp);

    std::array<int,2> target;
    for (auto [xy, v] : map)
      if (v == portals[{'Z','Z'}])
        queue.push({0, xy[0], xy[1]});
      else if (v == portals[{'A','A'}])
        target = xy;


    while (queue.size()) {
      auto [cost, x, y] = queue.top();
      queue.pop();

      if (visited.contains({x,y}))
        continue;
      visited.insert({x,y});

      if (oracle.contains({x,y}) and oracle[{x,y}] <= cost)
        continue;
      oracle[{x,y}] = cost;

      if (decltype(target){x, y} == target) {
        std::cout << cost << std::endl;
        break;
      }

      for (auto [dx,dy] : dxdy) {
        const auto tx = x+dx;
        const auto ty = y+dy;
        if (map[{tx,ty}] < 2)
          continue;
        queue.push({cost+1, tx, ty});
      }
      /// Traverse portal
      if (map[{x,y}] > 2)
        for (auto [xy, v] : map)
          if (v == map[{x,y}] and (x != xy[0] or y != xy[1]))
            queue.push({cost+1, xy[0], xy[1]});
    }
  }


  return EXIT_SUCCESS;
}

