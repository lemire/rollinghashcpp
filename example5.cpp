#include <string>
#include <memory>
#include <cassert>
#include <iostream>
#include "cyclichash.h"

// Define DNA's complementary nucleotides
# define nucleotide_complement(ch) ( \
	(toupper(ch)) == 'A' ? 'T' : \
	(toupper(ch)) == 'T' ? 'A' : \
	(toupper(ch)) == 'C' ? 'G' : 'C' \
)

// A sequence and its reverse complement (such as "GATTACA" and "TGTAATC") are
// biologically identical and should hash to the same value. A sequence that is
// equal to its reverse complement is a special case and should be handled
// accordingly.
#define canonical_hash(fwd, rev) ( \
    fwd == rev ? rev : fwd ^ rev \
)

#define WORDSIZE 5
#define SEED1 42
#define SEED2 1985
#define HASHBITS 64

int main(int argc, char * argv[])
{
    // With word size k=5, a DNA sequence with 7 nucleotides has 3 k-mers.
    string input = "GATTACA";
    string kmer1 = "GATTA";
    string kmer2 = "ATTAC";
    string kmer3 = "TTACA";

    // Initialize the hash function to compute the hash of the first k-mer.
    CyclicHash<uint64_t> forward(WORDSIZE, SEED1, SEED2, HASHBITS);
    CyclicHash<uint64_t> reverse(WORDSIZE, SEED1, SEED2, HASHBITS);
    for (int j = 0; j < 4; j++) {
        forward.eat(input[j]);
        reverse.eat(nucleotide_complement(input[3 - j]));
    }
    uint64_t hashval = canonical_hash(forward.hashvalue, reverse.hashvalue);
    std::cout << kmer1 << ' ' << hashval << '\n';

    // Now do rolling updates to the hash.
    forward.update('G', 'C');
    reverse.reverse_update(nucleotide_complement('G'), nucleotide_complement('C'));
    hashval = canonical_hash(forward.hashvalue, reverse.hashvalue);
    std::cout << kmer2 << ' ' << hashval << '\n';

    forward.update('A', 'A');
    reverse.reverse_update(nucleotide_complement('A'), nucleotide_complement('A'));
    hashval = canonical_hash(forward.hashvalue, reverse.hashvalue);
    std::cout << kmer3 << ' ' << hashval << '\n';

    // Finally, test that the rolling updates gives us the same hash as
    // initializing the third k-mer from scratch.
    CyclicHash<uint64_t> forward_test(WORDSIZE, SEED1, SEED2, HASHBITS);
    CyclicHash<uint64_t> reverse_test(WORDSIZE, SEED1, SEED2, HASHBITS);
    for (int j = 0; j < 4; j++) {
        forward_test.eat(kmer3[j]);
        reverse_test.eat(nucleotide_complement(kmer3[3 - j]));
    }
    uint64_t testhashval = canonical_hash(forward_test.hashvalue, reverse_test.hashvalue);
    std::cout << "Test: " << hashval << " == " << testhashval << " ?\n";
    assert(testhashval == hashval);

    return 0;
}
