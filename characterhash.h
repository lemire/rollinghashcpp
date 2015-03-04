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

template <typename hashvaluetype = uint32, typename chartype =  unsigned char>
class CharacterHash {
 public:
  CharacterHash(uint32 maxval) {
    cout<<sizeof(hashvaluetype)<<endl;
  	mersenneRNG randomgenerator(maxval);
  	for(uint32 k =0; k<nbrofchars; ++k) 
  	  if(sizeof(hashvaluetype) <= 4)
  	    hashvalues[k] = static_cast<hashvaluetype>(randomgenerator());
  	  else if (sizeof(hashvaluetype) <= 8)
  	    hashvalues[k] = static_cast<hashvaluetype>(randomgenerator()) 
  	     | (static_cast<hashvaluetype>(randomgenerator()) << 32);

  	  else if (sizeof(hashvaluetype) <= 16)
  	     hashvalues[k] = static_cast<hashvaluetype>(randomgenerator()) 
  	      | (static_cast<hashvaluetype>(randomgenerator()) << 32)
  	      | (static_cast<hashvaluetype>(randomgenerator()) << 64)
  	      | (static_cast<hashvaluetype>(randomgenerator()) << 96);
  	  else throw runtime_error("unsupported hash value type");
  }

  enum{nbrofchars = 1 << ( sizeof(chartype)*8 )};

  hashvaluetype hashvalues[1 << ( sizeof(chartype)*8 )];	
};

#endif

