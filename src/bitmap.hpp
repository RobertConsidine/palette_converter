#ifndef PALETTE_CONVERTER_BITMAP_HPP
#define PALETTE_CONVERTER_BITMAP_HPP

#include <cstdint>
#include "array.hpp"

using Bitmap = ContigMultiArray<uint8_t, 3>;

const uint32_t BMP_HEADER_SIZE = 54;

struct __attribute__ ((packed)) BmpFileHeader {
  BmpFileHeader(uint32_t w, uint32_t h, uint32_t channels)
    : size(BMP_HEADER_SIZE + w * h * channels) {}

  char type[2] = {'B', 'M'};
  uint32_t size;
  uint16_t reserved1 = 0;
  uint16_t reserved2 = 0;
  uint32_t offset = BMP_HEADER_SIZE;
};

struct __attribute__ ((packed)) BmpImgHeader {
  BmpImgHeader(uint32_t w, uint32_t h, uint32_t channels)
    : width(w),
      height(h),
      bitCount(channels * 8) {}

  uint32_t size = 40;
  uint32_t width;
  uint32_t height;
  uint16_t planes = 1;
  uint16_t bitCount;
  uint32_t compression = 0;
  uint32_t imgSize = 0;
  uint32_t xPxPerMetre = 0;
  uint32_t yPxPerMetre = 0;
  uint32_t colMapEntriesUsed = 0;
  uint32_t numImportantColours = 0;
};

struct __attribute__ ((packed)) BmpHeader {
  BmpHeader(uint32_t imgW, uint32_t imgH, int channels)
    : fileHdr(imgW, imgH, channels),
      imgHdr(imgW, imgH, channels) {}

  BmpFileHeader fileHdr;
  BmpImgHeader imgHdr;
};

Bitmap loadBitmap(const std::string& path);
void saveBitmap(const Bitmap& bitmap, const std::string& path);

#endif
