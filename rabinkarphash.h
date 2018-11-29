#ifndef KARPRABINHASH
#define KARPRABINHASH


#include "characterhash.h"


/**
* This is a randomized version of the Karp-Rabin hash function.
* Each instance is a rolling hash function meant to hash streams of characters.
* Each new instance of this class comes with new random keys.
*
* Recommended usage to get L-bit hash values over n-grams:
*        KarpRabinHash<> hf(n,L );
*        for(uint32 k = 0; k<n;++k) {
*                  unsigned char c = ... ; // grab some character
*                  hf.eat(c); // feed it to the hasher
*        }
*        while(...) { // go over your string
*           hf.hashvalue; // at all times, this contains the hash value
*           unsigned char c = ... ;// points to the next character
*           unsigned char out = ...; // character we want to forget
*           hf.update(out,c); // update hash value
*        }
*/
template <typename hashvaluetype = uint32, typename chartype =  unsigned char>
class KarpRabinHash {

public:
    // myn is the length of the sequences, e.g., 3 means that you want to hash sequences of 3 characters
    // mywordsize is the number of bits you which to receive as hash values, e.g., 19 means that the hash values are 19-bit integers
    KarpRabinHash(int myn, int mywordsize=19) :  hashvalue(0),n(myn),
        wordsize(mywordsize),
        hasher( maskfnc<hashvaluetype>(wordsize)),
        HASHMASK(maskfnc<hashvaluetype>(wordsize)),BtoN(1) {
        for (int i=0; i < n ; ++i) {
            BtoN *= B;
            BtoN &= HASHMASK;
        }
    }

    // prepare to process a new string, you will need to call "eat" again
    void reset() {
      hashvalue = 0;
    }

    // this is a convenience function, use eat,update and .hashvalue to use as a rolling hash function
    template<class container>
    hashvaluetype  hash(container & c) {
        hashvaluetype answer(0);
        for(uint k = 0; k<c.size(); ++k) {
            hashvaluetype x(1);
            for(uint j = 0; j< c.size()-1-k; ++j) {
                x= (x * B) & HASHMASK;
            }
            x= (x * hasher.hashvalues[c[k]]) & HASHMASK;
            answer=(answer+x) & HASHMASK;
        }
        return answer;
    }

    // add inchar as an input, this is used typically only at the start
    // the hash value is updated to that of a longer string (one where inchar was appended)
    void eat(chartype inchar) {
        hashvalue = (B*hashvalue +  hasher.hashvalues[inchar] )& HASHMASK;
    }

    // add inchar as an input and remove outchar, the hashvalue is updated
    // this function can be used to update the hash value from the hash value of [outchar]ABC to the hash value of ABC[inchar]
    void update(chartype outchar, chartype inchar) {
        hashvalue = (B*hashvalue +  hasher.hashvalues[inchar] - BtoN *  hasher.hashvalues[outchar]) & HASHMASK;
    }


    hashvaluetype hashvalue;
    int n;
    const int wordsize;
    CharacterHash<hashvaluetype,chartype> hasher;
    const hashvaluetype HASHMASK;
    hashvaluetype BtoN;
    static const hashvaluetype B=37;

};

template <typename hashvaluetype = uint32, typename chartype =  unsigned char, unsigned wordsize=CHAR_BIT * sizeof(hashvaluetype)>
class KarpRabinHashBits {
    // The key difference between KarpRabinHashBits and KarpRabinHash is that wordsize is now templated
    // And the masking is only performed if nbits != the number of bits in the type
public:
    // myn is the length of the sequences, e.g., 3 means that you want to hash sequences of 3 characters
    // mywordsize is the number of bits you which to receive as hash values, e.g., 19 means that the hash values are 19-bit integers
    KarpRabinHashBits(int myn):  hashvalue(0), n(myn),
        hasher( maskfnc<hashvaluetype>(wordsize)),
        HASHMASK(maskfnc<hashvaluetype>(wordsize)),BtoN(1) {
        for (int i=0; i < n ; ++i) {
            BtoN *= B;
            if(!is_full_word()) BtoN &= HASHMASK;
        }
    }

    // prepare to process a new string, you will need to call "eat" again
    void reset() {
      hashvalue = 0;
    }
    static constexpr bool is_full_word() {
        return wordsize == (CHAR_BIT * sizeof(hashvaluetype));
    }
    template<typename T>
    void mask_value(T &val) const {
        if constexpr(!is_full_word()) val &= HASHMASK;
    }

    // this is a convenience function, use eat,update and .hashvalue to use as a rolling hash function
    template<class container>
    hashvaluetype  hash(container & c) const {
        hashvaluetype answer(0);
        for(uint k = 0; k<c.size(); ++k) {
            hashvaluetype x(1);
            for(uint j = 0; j< c.size()-1-k; ++j) {
                x= (x * B);
                mask_value(x);
            }
            x= (x * hasher.hashvalues[c[k]]);
            mask_value(x);
            answer=(answer+x);
            mask_value(answer);
        }
        return answer;
    }
    hashvaluetype  hash(char *s) const {return hash(static_cast<const char *>(s));}
    hashvaluetype  hash(const char *s) const {
        hashvaluetype answer(0);
        uint csz = std::strlen(s);
        for(uint k = 0; k<csz; ++k) {
            hashvaluetype x(1);
            for(uint j = 0; j< csz-1-k; ++j) {
                x= (x * B);
                mask_value(x);
            }
            x= (x * hasher.hashvalues[s[k]]);
            mask_value(x);
            answer=(answer+x);
            mask_value(answer);
        }
        return answer;
    }

    // add inchar as an input, this is used typically only at the start
    // the hash value is updated to that of a longer string (one where inchar was appended)
    void eat(chartype inchar) {
        hashvalue = (B*hashvalue +  hasher.hashvalues[inchar] );
        mask_value(hashvalue);
    }

    // add inchar as an input and remove outchar, the hashvalue is updated
    // this function can be used to update the hash value from the hash value of [outchar]ABC to the hash value of ABC[inchar]
    void update(chartype outchar, chartype inchar) {
        hashvalue = (B*hashvalue +  hasher.hashvalues[inchar] - BtoN *  hasher.hashvalues[outchar]);
        mask_value(hashvalue);
    }


    hashvaluetype hashvalue;
    int n;
    CharacterHash<hashvaluetype,chartype> hasher;
    const hashvaluetype HASHMASK;
    hashvaluetype BtoN;
    static constexpr hashvaluetype B=37;
};


#endif
