#include <string>
#include <memory>
#include <cassert>
#include <iostream>
#include "cyclichash.h"

/*
An issue is application-specific and has to do with the nature of DNA. Even
though we usually represent DNA as a string of characters (such as `GATTACA`),
this is really only half the story. DNA is double stranded with `A` pairing
to `T` and `C` pairing to `G`, so the string `GATTACA` really represents the
following molecule.

```
gattaca
|||||||
ɔʇɐɐʇƃʇ
```

In most contexts, we have no way of knowing whether the original piece of DNA
sampled was from the top strand or the bottom strand, and so when we hash DNA
sequences we typically want the two complementary sequences to hash to the
same value.

I used two cyclic hashes: one for the "top" strand of DNA (observed from the
provided string, updated using forward updates) and one for the "bottom" strand
(inferred from the provided string, updated using reverse updates). Then to get
the hash for a particular k-mer (n-gram) in the DNA, I just XOR the current
forward and reverse hashes.
*/


// Define DNA's complementary nucleotides
//
// Daniel: This is probably  inefficient. Needlessly so.
// if efficiency matters, you want to define the character hash so that it takes the
// key 'A' to the hash value of 'T' and so forth.
//
# define nucleotide_complement(ch) ( \
	(toupper(ch)) == 'A' ? 'T' : \
	(toupper(ch)) == 'T' ? 'A' : \
	(toupper(ch)) == 'C' ? 'G' : 'C' \
)

// A sequence and its reverse complement (such as "GATTACA" and "TGTAATC") are
// biologically identical and should hash to the same value. A sequence that is
// equal to its reverse complement is a special case and should be handled
// accordingly.
//
#define canonical_hash(fwd, rev) ( \
    fwd == rev ? rev : fwd ^ rev \
)

#define WORDSIZE 5
#define SEED1 42
#define SEED2 1985
#define HASHBITS 64


// full string hash from scratch (for comparison)
uint64_t fullhash(const string & input) {
  assert(input.size() == WORDSIZE);
    CyclicHash<uint64_t> forward(input.size(), SEED1, SEED2, HASHBITS);
    CyclicHash<uint64_t> reverse(input.size(), SEED1, SEED2, HASHBITS);
    for (int j = 0; j < input.size(); j++) {
        forward.eat(input[j]);
        reverse.eat(nucleotide_complement(input[input.size() - 1 - j]));
    }
    return canonical_hash(forward.hashvalue, reverse.hashvalue);
}

// check the rolling hash
// k is the k-gram size, input is any string
void demo(int k, string input) {
    // Initialize the hash function to compute the hash of the first k-mer.
    CyclicHash<uint64_t> forward(k, SEED1, SEED2, HASHBITS);
    CyclicHash<uint64_t> reverse(k, SEED1, SEED2, HASHBITS);
    for (int j = 0; j < k; j++) {
        forward.eat(input[j]);
        // going backward
        reverse.eat(nucleotide_complement(input[k - 1 - j]));
    }
    // rolling has
    uint64_t hashval = canonical_hash(forward.hashvalue, reverse.hashvalue);
    assert(fullhash(input.substr(0,k)) == hashval);
    std::cout <<  input.substr(0,k) << " " << hashval << std::endl;

    for(int j = k ; j < input.size(); j++) {
      forward.update(input[j-k], input[j]);
      // note: you to flip the parameters of reverse_update
      reverse.reverse_update(nucleotide_complement(input[j]), nucleotide_complement(input[j-k]));
      // compute the rolling has
      hashval = canonical_hash(forward.hashvalue, reverse.hashvalue);
      // compare with full string hash
      assert(fullhash(input.substr(j-k+1,k)) == hashval);
      std::cout <<  input.substr(j-k+1,k) << " " << hashval << std::endl;
    }
}

int main(int argc, char * argv[])
{
    demo(5,"GATTACACAATAGCAAATT");
    std::cout << " code looks good " << std::endl;
    return 0;
}
