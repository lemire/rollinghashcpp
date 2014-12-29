#ifndef CYCLICHASH
#define CYCLICHASH

#include "characterhash.h"

/**
* Each instance is a rolling hash function meant to hash streams of characters.
* Each new instance of this class comes with new random keys.
*
* Recommended usage to get L-bit hash values over n-grams:
*        CyclicHash hf(n,L );
*        for(uint32 k = 0; k<n;++k) {
*                  chartype c = ... ; // grab some character
*                  hf.eat(c); // feed it to the hasher
*        }
*        while(...) { // go over your string
*           hf.hashvalue; // at all times, this contains the hash value
*           chartype c = ... ;// point to the next character
*           chartype out = ...; // character we want to forget
*           hf.update(out,c); // update hash value
*        }
*/
class CyclicHash {

  public:
    // myn is the length of the sequences, e.g., 3 means that you want to hash sequences of 3 characters 
    // mywordsize is the number of bits you which to receive as hash values, e.g., 19 means that the hash values are 19-bit integers
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
    
    void fastleftshiftn(hashvaluetype & x) const {
        x =  ((x & maskn) << myr ) | (x >> (wordsize-myr)) ;     }

    void fastleftshift(hashvaluetype & x, int r) const {
        r = r % wordsize;
        const uint32 mask = (static_cast<uint32>(1)<<(wordsize-r)) -1 ;
        x =  ((x & mask) << r ) | (x >> (wordsize-r)) ;
    }

    
    void fastleftshift1(hashvaluetype & x) const {
        x =  ((x & mask1) << 1 ) | (x >> (wordsize-1)) ;
    }
    
     // this is a convenience function, use eat,update and .hashvalue to use as a rolling hash function 
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
    
    // add inchar as an input and remove outchar, the hashvalue is updated
    void update(chartype outchar, chartype inchar) {
      hashvaluetype z (hasher.hashvalues[outchar]);
      fastleftshiftn(z);
      hashvalue =   ( ((hashvalue & mask1) << 1 ) | (hashvalue >> (wordsize-1)) )  
      ^ z
      ^ hasher.hashvalues[inchar];
    }
    
    
    // add inchar as an input, this is used typically only at the start
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
