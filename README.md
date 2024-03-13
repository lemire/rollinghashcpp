# Randomized rolling hash functions in C++
[![Ubuntu 22.04 CI (GCC 11)](https://github.com/lemire/rollinghashcpp/actions/workflows/ubuntu22.yml/badge.svg)](https://github.com/lemire/rollinghashcpp/actions/workflows/ubuntu22.yml)

License: Apache 2.0


## What is this?

This is a set of C++ classes implementing various recursive n-gram hashing techniques, also called rolling hashing (http://en.wikipedia.org/wiki/Rolling_hash), including:

*   Randomized Karp-Rabin (sometimes called Rabin-Karp)
*   Hashing by Cyclic Polynomials (also known as Buzhash)
*   Hashing by Irreducible Polynomials

This library is used by [khmer](https://github.com/dib-lab/khmer/): the in-memory nucleotide sequence k-mer engine.
 

These are randomized hash functions, meaning that each time you create a new hasher instance, you will
get new hash values for a given input.

##  Code sample
```cpp

        const uint n(3);//hash all sequences of 3 characters
        const uint L(7); // you need 7 bits
        CyclicHash<uint32> hf(n,L );// if you want 64-bit values replace uint32 by uint64
        for(uint32 k = 0; k<n;++k) {
                  chartype c = ... ; // grab some character
                  hf.eat(c); // feed it to the hasher
        }
        while(...) { // go over your string
           hf.hashvalue; // at all times, this contains the hash value
           chartype c = ... ;// point to the next character
           chartype out = ...; // character we want to forget
           hf.update(out,c); // update hash value
        }
        hf.reset(); // you can now hash a new string
```


##  Requirements

A recent GNU GCC C++ compiler or a recent CLANG.

##  What should I do after I download it?

It is a conventional Cmake projet.

```
cmake -B build
cmake --build build
ctest --test-dir build
```


## Nim version

See [Cyclic-Polynomial-Hash](https://github.com/MarcAzar/Cyclic-Polynomial-Hash) for a similar library written in Nim.

##  References

* Daniel Lemire, Owen Kaser: Recursive n-gram hashing is pairwise independent, at best, Computer Speech & Language, Volume 24, Issue 4, October 2010, Pages 698-710 http://arxiv.org/abs/0705.4676
* Daniel Lemire, The universality of iterated hashing over variable-length strings, Discrete Applied Mathematics 160 (4-5), 2012. http://arxiv.org/abs/1008.1715
* Owen Kaser and Daniel Lemire, Strongly universal string hashing is fast, Computer Journal (2014) 57 (11): 1624-1638. http://arxiv.org/abs/1202.4961


This work has been used in genomics, see


* Ilia Minkin, Son Pham, Paul Medvedev, TwoPaCo: an efficient algorithm to build the compacted de Bruijn graph from many complete genomes, Bioinformatics (to appear). https://doi.org/10.1093/bioinformatics/btw609 and http://github.com/medvedevgroup/TwoPaCo
* Xiaofei Zhao, BinDash, software for fast genome distance estimation on a typical personal laptop, Bioinformatics. https://academic.oup.com/bioinformatics/article/35/4/671/5058094?login=true and https://github.com/zhaoxiaofei/bindash?tab=readme-ov-file 
