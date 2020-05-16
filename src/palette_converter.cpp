#include <iostream>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>
#include "bitmap.hpp"
#include "exception.hpp"

namespace po = boost::program_options;

static const std::string DESCRIPTION =
  "Palette Converter replaces the colours in a bitmap image to conform to "
  "the given colour palette.";

struct Colour {
  explicit Colour(const ContigMultiArray<uint8_t, 1>& array) {
    r = array[2];
    g = array[1];
    b = array[0];
  }

  explicit Colour(uint32_t value) {
    r = value >> 16;
    g = (value & 0xff00) >> 8;
    b = value & 0xff;
  }

  uint32_t value() const {
    return (r << 16) | (g << 8) | b;
  }

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

using Palette = std::vector<Colour>;

Palette paletteFromFile(const std::string& path) {
  std::ifstream fin(path);
  if (!fin.good()) {
    EXCEPTION("Error opening file " << path);
  }

  Palette palette;

  char buffer[32];
  std::stringstream ss;
  uint32_t value;

  while (!fin.eof()) {
    fin.getline(buffer, sizeof(buffer));

    ss.str("");
    ss.clear();

    ss << std::hex << buffer;
    ss >> value;
    if (!ss.fail()) {
      palette.push_back(Colour(value));
    }
  }

  return palette;
}

size_t distance(const Colour& a, const Colour& b) {
  return (a.r - b.r) * (a.r - b.r) +
         (a.g - b.g) * (a.g - b.g) +
         (a.b - b.b) * (a.b - b.b);
}

Colour chooseColour(const Palette& palette, const Colour& colour) {
  Colour closestColour(0);
  uint32_t closestDistance = -1;

  for (auto& swatch : palette) {
    size_t dist = distance(swatch, colour);
    if (dist < closestDistance) {
      closestColour = swatch;
      closestDistance = dist;
    }
  }

  return closestColour;
}

void doConversion(Bitmap& bitmap, const Palette& palette) {
  size_t w = bitmap.size()[0];
  size_t h = bitmap.size()[1];
  for (size_t i = 0; i < w; ++i) {
    for (size_t j = 0; j < h; ++j) {
      Colour pixel(bitmap[i][j]);
      Colour convertedPixel = chooseColour(palette, pixel);

      bitmap[i][j][0] = convertedPixel.b;
      bitmap[i][j][1] = convertedPixel.g;
      bitmap[i][j][2] = convertedPixel.r;
    }
  }
}

int main(int argc, char** argv) {
  try {
    po::options_description desc{DESCRIPTION};
    desc.add_options()
      ("help,h", "Show help")
      ("palette,p", po::value<std::string>()->required(),
       "Text file containing list of hex colour values separated by new lines")
      ("file,f", po::value<std::string>()->required(), "Bitmap image file")
      ("output,o", po::value<std::string>()->default_value("converted.bmp"),
       "Output file name");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }

    po::notify(vm);

    std::string bmpPath = vm["file"].as<std::string>();
    std::string palettePath = vm["palette"].as<std::string>();
    std::string outputPath = vm["output"].as<std::string>();

    auto bitmap = loadBitmap(bmpPath);
    auto palette = paletteFromFile(palettePath);

    doConversion(bitmap, palette);
    saveBitmap(bitmap, outputPath);
  }
  catch (const po::error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
