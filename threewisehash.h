#ifndef THREEWISEHASH
#define THREEWISEHASH

#include <deque>
#include <vector>
#include "characterhash.h"

using namespace std;

class ThreeWiseHash {

  public:
    
    ThreeWiseHash(int myn, int mywordsize=19) : n(myn), wordsize(mywordsize), 
      hashers(),hasher(0){
      if(static_cast<uint>(wordsize) > 8*sizeof(hashvaluetype)) {
      	cerr<<"Can't create "<<wordsize<<"-bit hash values"<<endl;
      	throw "abord";
      } 
      for (int i=0; i < n; ++i) {
      	  CharacterHash ch(( 1<<wordsize ) - 1);
	      hashers.push_back(ch); 
      }
    }
    
    void eat(chartype inchar) {
    	ngram.push_back(inchar);
    	__updateHashValue();
    }
    
    inline void update(chartype outchar, chartype inchar) {
    	ngram.push_back(inchar);
    	ngram.pop_front();
    	__updateHashValue();
    }
    
    inline void __updateHashValue() {    
    	hashvalue = 0;
    	for(uint32 k = 0; k<ngram.size(); ++k) {
    		hashvalue ^= hashers[k].hashvalues[ngram[k]];
    	}
    }
    
    template<class container>
    hashvaluetype hash(container & c) {
    	hashvaluetype answer(0);
    	for(uint32 k = 0; k<ngram.size(); ++k) {
    		hashvalue ^= hashers[k].hashvalues[c[k]];
    	}
    	return answer;
    }
    
    hashvaluetype hashvalue;
    const int n, wordsize;
    deque<chartype> ngram;
    vector<CharacterHash> hashers;
    CharacterHash hasher;//placeholder
    
};


#endif

