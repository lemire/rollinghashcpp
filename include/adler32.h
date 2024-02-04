#include <stdint.h>

// contributed by Dmitry Artamonov
// this is *deterministic*
class Adler32 {
  uint32 sum1, sum2;

public:
  static const uint32_t Base = 65521;
  uint32_t hashvalue;
  int len;

  Adler32(int window) : sum1(1), sum2(0), hashvalue(0), len(window) {}

  void eat(uint8_t inchar) {
    sum1 = (sum1 + inchar) % Base;
    sum2 = (sum2 + sum1) % Base;

    hashvalue = (sum2 << 16) | sum1;
  }

  void reset() {
    sum1 = 1;
    sum2 = 0;
    hashvalue = 0;
  }

  void update(uint8_t outchar, uint8_t inchar) {
    int sum2 = (hashvalue >> 16) & 0xffff;
    int sum1 = hashvalue & 0xffff;

    sum1 += inchar - outchar;
    if (sum1 >= Base) {
      sum1 -= Base;
    } else if (sum1 < 0) {
      sum1 += Base;
    }

    sum2 = ((int)(sum2 - len * outchar + sum1 - 1) % (int)Base);
    if (sum2 < 0) {
      sum2 += Base;
    }
    hashvalue = (sum2 << 16) | sum1;
  }
};
