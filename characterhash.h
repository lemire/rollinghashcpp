#ifndef CHARACTERHASH
#define CHARACTERHASH

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned int uint;

#include <cassert>
#include <iostream>

#include "mersennetwister.h"

using namespace std;

class mersenneRNG {
 public:
  mersenneRNG(uint32 maxval) : mtr(),n(maxval) {};
  uint32 operator()() { return mtr.randInt(n);} 
  void seed(uint32 seedval) { mtr.seed(seedval);}
  void seed() { mtr.seed();}
  uint32 rand_max() { return n;}
 private:
  MTRand mtr;
  int n;
};

typedef uint32 hashvaluetype;
typedef unsigned char chartype;


class CharacterHash {
 public:
  CharacterHash(uint32 maxval) {
  	mersenneRNG randomgenerator(maxval);
  	for(uint32 k =0; k<nbrofchars; ++k) 
  	  hashvalues[k] = randomgenerator();
  }

  enum{nbrofchars = 1 << ( sizeof(chartype)*8 )};

  hashvaluetype hashvalues[1 << ( sizeof(chartype)*8 )];	
};

#endif

