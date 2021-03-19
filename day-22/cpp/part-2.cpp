#include <cmath>
#include <cstdlib>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

/**
 * DISCLAIMER
 *
 * This is not my solution.
 *
 * The math in this one is beyond me and I admit defeat.
 *
 * I have absolutely no idea how the trick with the modular
 * exponentiation of "length - 2" works.
 *
 * DISCLAIMER
 */

int main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  constexpr __int128_t LEN    = 119'315'717'514'047;
  constexpr __int128_t ROUNDS = 101'741'582'076'661;
  constexpr __int128_t index  = 2020;

  /// Linear map y = mx + a
  auto m = 1ll;
  auto a = 0ll;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    if (line == "deal into new stack") {
      m = (-m + LEN) % LEN;
      a = (LEN - a - 1) % LEN;
    } else {
      std::istringstream iss{line};
      std::string s;
      long long i;
      iss >> s;
      if (s == "cut") {
        iss >> i;
        a = (a - i) % LEN;
      } else {
        iss >> s >> s;
        iss >> i;
        m = (m * i + LEN) % LEN;
        a = (a * i + LEN) % LEN;
      }
    }
  }

  std::function<__int128_t(__int128_t, __int128_t, __int128_t)> modexp;
  modexp = [&modexp](__int128_t a, __int128_t b, __int128_t c) {
    if (b == 0)
      return (__int128_t)1;
    else if (b == 1)
      return a % c;
    else {
      __int128_t t = modexp(a, b/2, c);
      t = (t * t) % c;
      if (b % 2 == 0)
        return t;
      else
        return ((a % c) * t) % c;
    }
  };

  __int128_t r = (a * modexp(1-m, LEN-2, LEN)) % LEN;
  std::cout << (long long)(((index - r) * modexp(m, ROUNDS*(LEN-2), LEN) + r) % LEN) << std::endl;

  return EXIT_SUCCESS;
}

