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
//U8 operator""_8(unsigned long long v);

struct State
{
  std::array<U8,2>  xy;
  KEY keys;

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
  os << n.cost << "\t" << n.heuristic << "\t"
     << "(" << (int)n.state.xy[0] << "," << (int)n.state.xy[1] << ") "
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
  std::map<KEY, std::array<U8,2>> keys;
  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
      if (map[{x,y}] == '@')
        queue.push({0, 1, {{x,y}, 0}});
      else if (auto k = is_key(map[{x,y}]); k > 0)
        keys[k] = {x,y};

  auto heuristic = [&](const State& s) {
    auto h = 0;
    for (auto [k, xy] : keys)
      if ((k & s.keys) == 0)
        if (auto d = std::abs(s.xy[0] - xy[0]) + std::abs(s.xy[1] - xy[1]);
            d < h or h == 0) {
          h = d;
        }
    return h;
  };

  {
    auto p = queue.top();
    queue.pop();
    Node n = p;
    n.heuristic = heuristic(n.state);
    queue.push(n);
  }

  std::set<State> visited;

  const auto dxdy = std::to_array<std::array<int,2>>({
    {0,-1}, {1,0}, {0,1}, {-1,0}
  });

  while (queue.size()) {
    auto node = queue.top();
    queue.pop();

    if (visited.contains(node.state))
      continue;
    visited.insert(node.state);

    //std::cout << node << std::endl;

    if (node.heuristic == 0) {
      std::cout << node.cost << std::endl;
      break;
    }

    x = node.state.xy[0];
    y = node.state.xy[1];

    for (auto [dx,dy] : dxdy) {
      const auto m = map[{x+dx,y+dy}];
      if (m == '#')
        continue;
      if (auto g = is_gate(m); 
          g > 0 and (g & node.state.keys) == 0)
        continue;

      auto next = node;
      next.cost++;
      next.state.xy[0] += dx;
      next.state.xy[1] += dy;
      if (auto k = is_key(m))
        next.state.keys |= k;
      next.heuristic = heuristic(next.state);
      queue.push(next);
    }
  }
  
}

