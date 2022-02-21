// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <cstring>
#include <fstream>
#include <iostream>

class SteganographicImage {
 private:
  std::string filename;

  int start = 0;
  int stride = 1;

  int w, h;
  size_t s;
  uint8_t *rgb, *out;

 public:
  SteganographicImage(std::string filename, int start, int stride)
      : filename(filename), start(start), stride(stride) {
    std::ifstream f(filename, std::ios::binary | std::ios::in);
    if (!f) throw "Input file '" + filename + "' does not exist.";

    // Get byte size of file and return for reading.
    f.seekg(0, std::ios::end);
    s = f.tellg();
    f.clear();
    f.seekg(0);

    uint8_t *img = new uint8_t[s];
    f.read((char *)img, s);

    if (!WebPGetInfo(img, s, &w, &h))
      throw "Input image is not a valid WebP file.";
    rgb = WebPDecodeRGB(img, s, &w, &h);
    delete[] img;
  }

  ~SteganographicImage() { WebPFree(rgb); }

  void hide(char *str) {
    if (start + strlen(str) * stride > s)
      throw "Input string is too long or stride and start are too large to fit in the image.";
    char bit = 0, c = *str++;
    int i;
    for (i = start; i < s && c; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *str++;
    }
    // HACK: For correctly encoding the null-byte of strings.
    if (!c)
      for (int j = 0; i < s || j == 8; i += stride, ++j) rgb[i] &= ~1;
  }

  void write() {
    // NOTE: Doesn't work with transparent webps.
    s = WebPEncodeLosslessRGB(rgb, w, h, w * 3, &out);

    std::ofstream f("new_" + filename, std::ios::binary | std::ios::out);
    f.write((char *)out, s);
    WebPFree(out);
  }

  std::string recover() {
    char bit = 0, c = 0;
    std::string str;
    for (int i = start; i < s; i += stride) {
      if (rgb[i] & 1) c |= 1;
      if (++bit == 8) {
        if (!c) {
          break;
        }
        bit = 0, str += c, c = 0;
      } else
        c <<= 1;
    }
    return str;
  }
};

int main(int argc, char **argv) {
  if (argc < 3 || (argc == 4 && argv[1][0] != 'h') ||
      (argc != 4 && argv[1][0] == 'h') || (argc != 3 && argv[1][0] == 'r')) {
    std::cerr << "Usage: " << argv[0]
              << " [h|r] <input.webp> 'string to hide'\n    "
                 "h: Hide string in given image.\n    "
                 "r: Recover string from given image."
              << std::endl;
    return 1;
  }

  try {
    // TODO:
    // - Use a seed to one-time randomize info start and offset.
    // - Deterministically decide start/stride params based on side of image.
    // - Maybe combine both of these to create a sort of random tolerance.
    SteganographicImage steg(argv[2], 200, 5000);
    switch (argv[1][0]) {
      case 'h':
        steg.hide(argv[3]);
        steg.write();
        break;
      case 'r':
        std::cout << "Recovered message: " << steg.recover() << std::endl;
        break;
      default:
        std::cerr << "Error: Invalid command: " << argv[1] << std::endl;
        std::cerr << "Usage: " << argv[0] << " [h|r] <input.webp>" << std::endl;
        return 1;
    }
  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }
  return 0;
}