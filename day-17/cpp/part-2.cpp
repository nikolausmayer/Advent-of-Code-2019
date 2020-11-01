
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "intcode.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  std::vector<Intcode::DATA_t> program;
  {
    auto filename = argv[1];
    std::ifstream infile{filename};
    if (infile.bad() or not infile.is_open()) {
      throw std::runtime_error("cannot open file");
    }
    Intcode::DATA_t opcode{0};
    auto negative{false};
    for (;;) {
      auto c{infile.get()};
      if (infile.eof()) {
         program.push_back(negative ? -opcode : opcode);
        break; 
      }
       if (c == ',' or c == '\n') {
         program.push_back(negative ? -opcode : opcode);
        opcode = 0;
        negative = false;
      } else if (c == '-') {
        negative = true;
      } else {
         opcode = 10*opcode + (c - 48);
      }
    }
  }
  const auto program_save{program};


  std::vector<std::string> map;
  auto print_map = [&map] {
    for (const auto& line : map)
      std::cout << line << std::endl;
  };


  /// Run program once to get the initial map
  {
    Intcode robot{program};
    robot.run();

    {
      map.push_back("");

      Intcode::DATA_t pixel;
      while (robot.getOutput(pixel)) {
        if (pixel == '\n')
          map.push_back("");
        else
          map.back() = map.back() + std::string{(char)pixel};

        if (pixel == 'X')
          throw std::runtime_error("Robot fell off scaffold!");
      }
      map.pop_back();
      map.pop_back();
    }
  }

  /// Find robot position and orientation
  enum { N=0, E, S, W};
  int rx{-1}, ry, dir;
  for (auto y{0u}; y < map.size(); ++y) {
    for (auto x{0u}; x < map[0].size(); ++x) {
      switch (map[y][x]) {
        case '^': { rx = x; ry = y; dir = N; break; }
        case '>': { rx = x; ry = y; dir = E; break; }
        case 'v': { rx = x; ry = y; dir = S; break; }
        case '<': { rx = x; ry = y; dir = W; break; }
      }
    }
  }

  std::string full_path;

  auto getTile = [&map](auto x, auto y) {
    if (x < 0 or x >= map[0].size() or
        y < 0 or y >= map.size())
      return '0';
    else
      return map[y][x];
  };

  auto tile_ahead = [&dir, &rx, &ry, &map, &getTile] {
    return (dir == N) ? getTile(rx, ry-1) :
           (dir == E) ? getTile(rx+1, ry) :
           (dir == S) ? getTile(rx, ry+1) :
         /*(dir == W)*/ getTile(rx-1, ry);
  };

  auto tile_left = [&dir, &rx, &ry, &map, &getTile] {
    return (dir == N) ? getTile(rx-1, ry) :
           (dir == E) ? getTile(rx, ry-1) :
           (dir == S) ? getTile(rx+1, ry) :
         /*(dir == W)*/ getTile(rx, ry+1);
  };

  auto tile_right = [&dir, &rx, &ry, &map, &getTile] {
    return (dir == N) ? getTile(rx+1, ry) :
           (dir == E) ? getTile(rx, ry+1) :
           (dir == S) ? getTile(rx-1, ry) :
         /*(dir == W)*/ getTile(rx, ry-1);
  };

  auto turn_right = [&dir, &full_path] {
    dir = (dir+1) % 4;
    full_path = full_path + "R,";
  };
  auto turn_left = [&dir, &full_path] {
    dir = (dir-1+4) % 4;
    full_path = full_path + "L,";
  };
  auto step_forward = [&dir, &rx, &ry, &full_path] {
    rx += (dir == W) ? -1 :
          (dir == E) ?  1 : 0;
    ry += (dir == N) ? -1 :
          (dir == S) ?  1 : 0;
    full_path = full_path + "s";
  };


  // Walk grid to get a raw path representation
  for (;;) {
    while (tile_ahead() == '#')
      step_forward();

    if (tile_left() == '#')
      turn_left();
    else if (tile_right() == '#')
      turn_right();
    else
      break;
  }

  // Collapse path
  {
    std::ostringstream oss;
    auto streak{0u};
    for (const auto& c : full_path) {
      switch (c) {
        case 'L':
        case 'R':
        case ',': {
          if (streak > 0)
            oss << streak << ',';
          streak = 0;
          oss << c;
          break;
        }
        case 's': {
          ++streak;
          break;
        }
      }
    }
    if (streak > 0)
      oss << streak << ',';
    full_path = oss.str();
  }
  
  //std::cout << full_path << std::endl;
  
  auto remove_redundancies = [&full_path] (auto replacement) {
    auto maxlen{0u};
    auto maxstart{0u};
    for (auto s1{0u}; s1 < full_path.size()-1; ++s1) {
      for (auto s2{s1+3}; s2 < full_path.size()-1; ++s2) {
        for (auto idx{0u}; idx <= 20; ++idx) {
          if (full_path[s1+idx] == 'A' or 
              full_path[s1+idx] == 'B' or 
              full_path[s1+idx] == 'C' or 
              full_path[s2+idx] == 'A' or 
              full_path[s2+idx] == 'B' or 
              full_path[s2+idx] == 'C' or 
              full_path[s1] == ',' or 
              idx > s2-s1 or
              idx >= full_path.size() or
              full_path[s1+idx] != full_path[s2+idx]) {
            break;
          } else {
            if (idx > maxlen) {
              maxlen = idx;
              maxstart = s1;
            }
          }
        }
      }
    }
    while (full_path.substr(maxstart+maxlen, 2) != ",L" and
           full_path.substr(maxstart+maxlen, 2) != ",R" and 
           full_path.substr(maxstart+maxlen, 2) != ",A" and 
           full_path.substr(maxstart+maxlen, 2) != ",B" and
           full_path.substr(maxstart+maxlen, 2) != ",C")
      --maxlen;

    std::string part{full_path.substr(maxstart, maxlen)};
    while (full_path.find(part) != std::string::npos)
      full_path.replace(full_path.find(part), 
                        maxlen,
                        replacement);
    return part;
  };

  // Reduce path by identifying repeating elements
  auto A = remove_redundancies("A");
  auto B = remove_redundancies("B");
  auto C = remove_redundancies("C");
  full_path = full_path.substr(0, full_path.size()-1);
  //std::cout << full_path << std::endl
  //          << A << std::endl
  //          << B << std::endl
  //          << C << std::endl;
  


  program = program_save;
  program.at(0) = 2;
  Intcode robot{program};

  for (const auto& c : full_path) robot.setInput(c);
  robot.setInput(10);
  for (const auto& c : A) robot.setInput(c);
  robot.setInput(10);
  for (const auto& c : B) robot.setInput(c);
  robot.setInput(10);
  for (const auto& c : C) robot.setInput(c);
  robot.setInput(10);

  robot.setInput('n');
  robot.setInput(10);

  robot.run();

  std::cout << robot.getLastOutput() << std::endl;
}

