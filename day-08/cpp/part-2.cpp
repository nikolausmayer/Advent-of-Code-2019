
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  const std::string encoded = [&infile]{
    std::string tmp;
    infile >> tmp;
    return tmp;
  }();

  const auto W{25};
  const auto H{6};
  const auto length{encoded.size()};

  std::ofstream image{"image.pgm"};
  image << "P5" << " " << W << " " << H << " " << 255 << '\n';

  for (unsigned y{0}; y < H; ++y) {
    for (unsigned x{0}; x < W; ++x) {
      unsigned char pixel{2};
      for (unsigned long idx{y*W+x}; idx < length; idx += W*H) {
        switch (encoded.data()[idx]) {
          case '0': { pixel = 0; break; }
          case '1': { pixel = 255; break; }
        }
        if (pixel != 2) break;
      }
      image << pixel;
    }
  }
  image.close();

  std::cout << "Result written to \"image.ppm\"" << std::endl;
}

