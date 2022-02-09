// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>

uint8_t *hideStr(uint8_t *data, char *str) { return 0; }

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.webp>" << std::endl;
    return 1;
  }

  std::ifstream fi(argv[1]);

  // Get byte size of file
  fi.seekg(0, std::ios::end);
  size_t s = fi.tellg();
  std::cout << s << std::endl;
  fi.seekg(std::ios::end, std::ios::beg);

  uint8_t *data = new uint8_t[s];
  fi.read((char *)data, s);
  std::cout << "Image data: " << data << std::endl;

  int w, h;
  WebPGetInfo(data, s, &w, &h);
  std::cout << w << std::endl;
  std::cout << h << std::endl;

  WebPDecodeRGB(data, s, &w, &h);

  char *str = (char *)"hello world";
  hideStr(data, str);

  uint8_t *out;
  s = WebPEncodeLosslessRGB(data, w, h, w * 3, &out);
  std::ofstream fo("new.webp", std::ios::binary);
  fo.write((char *)out, s);
  delete[] data;
  // Note these functions, like the lossy versions, use the library's default
  // settings. For lossless this means 'exact' is disabled. RGB values in
  // transparent areas will be modified to improve compression. To avoid this,
  // use WebPEncode() and set WebPConfig::exact to 1.
  /* WebPEncodeLosslessRGB(rgb, w, h, 0, output); */
  return 0;
}