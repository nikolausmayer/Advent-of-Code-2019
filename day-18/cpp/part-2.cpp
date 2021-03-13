#include <cstdlib>
#include <array>
#include <bitset>
#include <compare>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

typedef uint_least8_t U8;
typedef std::uint32_t KEY;

struct State
{
  std::array<std::array<U8,2>,4> xys;
  KEY keys;
  std::size_t mover;

  auto operator<=>(const State& s) const = default;
};

struct Node
{
  unsigned cost;
  unsigned heuristic;
  State state;
};

std::ostream& operator<<(std::ostream& os, const Node& n)
{
  os << n.cost << "  " << n.heuristic << "  " << "(";
  for (const auto& xy : n.state.xys)
    os << "(" << (int)xy[0] << "," << (int)xy[1] << ")";
  os << ") [" 
     << n.state.mover << "] "
     << std::bitset<26>{n.state.keys};
  return os;
}


KEY is_gate(char c)
{
  if (c >= 'A' and c <= 'Z') {
    return (1 << (c - 'A'));
  } else {
    return 0;
  }
}

KEY is_key(char c)
{
  if (c >= 'a' and c <= 'z') {
    return (1 << (c - 'a'));
  } else {
    return 0;
  }
}


int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::map<std::tuple<int,int>, char> map;

  U8 y = 0;
  U8 x = 0;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    x = 0;
    for (auto c : line)
      map[{x++,y}] = c;
    y++;
  }
  const auto W = x;
  const auto H = y;


  auto cmp = [](const Node& a, const Node& b) {
    return (a.cost + a.heuristic) > (b.cost + b.heuristic);
  };
  std::priority_queue<Node,
                      std::vector<Node>,
                      decltype(cmp)> queue{cmp};

  KEY full_keychain = 0;
  std::vector<U8> portals;
  std::array<std::map<KEY, std::array<U8,2>>,4> quadrant_keys;
  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
      if (map[{x,y}] == '@') {
        portals.push_back(x);
        portals.push_back(y);
      } else if (auto k = is_key(map[{x,y}]); k > 0) {
        auto quad = (x < W/2 and y < H/2 ? 0 :
                     x > W/2 and y < H/2 ? 1 :
                     x < W/2 and y > H/2 ? 2 :
                                           3);
                      
        full_keychain |= k;
        quadrant_keys[quad][k] = {x,y};
        //std::cout << (int)k << " at (" << (int)x << "," << (int)y << ") in quadrant " << quad << std::endl;
      }

  auto heuristic = [&](const State& s) {
    auto h = 0;
    /*for (auto quad = 0; quad < 4; quad++)*/ {
      auto qh = 0;
      const auto& rxy = s.xys[s.mover];
      for (auto [k, kxy] : quadrant_keys[s.mover])
        if ((k & s.keys) == 0)
          if (auto d = std::abs(rxy[0] - kxy[0]) + 
                       std::abs(rxy[1] - kxy[1]);
              d < qh or qh == 0)
            qh = d;
      h += qh;
    }
    return h;
  };

  {
    Node n;
    n.cost = 0;
    n.state.xys[0] = {portals[0], portals[1]};
    n.state.xys[1] = {portals[2], portals[3]};
    n.state.xys[2] = {portals[4], portals[5]};
    n.state.xys[3] = {portals[6], portals[7]};
    n.state.keys = 0;
    for (auto quad = 0u; quad < 4; quad++) {
      auto copy = n;
      copy.state.mover = quad;
      copy.heuristic = heuristic(copy.state);
      queue.push(copy);
    }
  }

  std::set<State> visited;

  const auto dxdy = std::to_array<std::array<int,2>>({
    {0,-1}, {1,0}, {0,1}, {-1,0}
  });

  while (queue.size()) {
    auto node = queue.top();
    queue.pop();

    if (node.state.keys == full_keychain) {
      std::cout << node.cost << std::endl;
      break;
    }

    if (visited.contains(node.state))
      continue;
    visited.insert(node.state);

    auto quad = node.state.mover;
    {
      x = node.state.xys[quad][0];
      y = node.state.xys[quad][1];

      for (auto [dx,dy] : dxdy) {
        const auto m = map[{x+dx,y+dy}];
        if (m == '#')
          continue;
        if (auto g = is_gate(m); 
            g > 0 and (g & node.state.keys) == 0)
          continue;

        auto next = node;
        next.cost++;
        next.state.xys[quad][0] += dx;
        next.state.xys[quad][1] += dy;
        bool key_is_new = false;
        if (auto k = is_key(m); (next.state.keys & k) == 0) {
          next.state.keys |= k;
          key_is_new = true;
        }
        next.heuristic = heuristic(next.state);

        if (not is_key(m) or not key_is_new) {
          queue.push(next);
        } else {
          for (auto nmover = 0u; nmover < 4; nmover++) {
            auto copy = next;
            copy.state.mover = nmover;
            copy.heuristic = heuristic(copy.state);
            queue.push(copy);
          }
        }
      }
    }
  }
  
}

