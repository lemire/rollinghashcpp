#ifndef CYCLICHASH
#define CYCLICHASH

#include "characterhash.h"


class CyclicHash {

  public:
    CyclicHash(int myn, int mywordsize=19) : hashvalue(0), 
    n(myn), wordsize(mywordsize), 
      hasher( ( 1<<wordsize ) - 1),
      mask1((static_cast<hashvaluetype>(1)<<(wordsize-1)) -1),
      myr(n%wordsize),
      maskn((static_cast<uint32>(1)<<(wordsize-myr)) -1 )
      {
       if(static_cast<uint>(wordsize) > 8*sizeof(hashvaluetype)) {
      	cerr<<"Can't create "<<wordsize<<"-bit hash values"<<endl;
      	throw "abord";
      } 
    }
    
    inline void fastleftshiftn(hashvaluetype & x) const {
        x =  ((x & maskn) << myr ) | (x >> (wordsize-myr)) ;     }

    inline void fastleftshift(hashvaluetype & x, int r) const {
        r = r % wordsize;
        const uint32 mask = (static_cast<uint32>(1)<<(wordsize-r)) -1 ;
        x =  ((x & mask) << r ) | (x >> (wordsize-r)) ;
    }

    
    inline void fastleftshift1(hashvaluetype & x) const {
        x =  ((x & mask1) << 1 ) | (x >> (wordsize-1)) ;
    }
    
    
    template<class container>
    hashvaluetype  hash(container & c) {
    	assert(c.size()==static_cast<uint>(n));
    	hashvaluetype answer(0);
    	for(uint k = 0; k<c.size();++k) {
    		fastleftshift(answer, 1) ;
    		answer ^= hasher.hashvalues[c[k]];
    	}
    	return answer;
    }
    
    
    inline void update(chartype outchar, chartype inchar) {
      hashvaluetype z (hasher.hashvalues[outchar]);
      fastleftshiftn(z);
      hashvalue =   ( ((hashvalue & mask1) << 1 ) | (hashvalue >> (wordsize-1)) )  
      ^ z
      ^ hasher.hashvalues[inchar];
    }
    
    
    
    void eat(chartype inchar) {
      fastleftshift1(hashvalue);
      hashvalue ^= hasher.hashvalues[inchar];
    }
  
    uint32 hashvalue;
    const int n, wordsize;
    CharacterHash hasher;
    const hashvaluetype mask1;
    const int myr;
    const hashvaluetype maskn;

};



#endif
