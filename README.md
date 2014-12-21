# Testing n-gram hash functions
Daniel Lemire

License: Apache 2.0


What is this?

This is a set of C++ classes implementing various recursive n-gram hashing techniques, also called rolling hashing (http://en.wikipedia.org/wiki/Rolling_hash), including:

*   Randomized Karp-Rabin (sometimes called Rabin-Karp)
*   Hashing by Cyclic Polynomials (also known as Buzhash)
*   Hashing by Irreducible Polynomials


##  Code sample

        const uint n(3);//hash all sequences of 3 characters
        const uint L(7); // you need 7 bits
        CyclicHash hf(n,L );
        for(uint32 k = 0; k<n;++k) {
                  chartype c = ... ; // grab some character
                  hf.eat(c); // feed it to the hasher
        }
        while(...) { // go over your string
           chartype c = ... ;// point to the next character
           chartype out = ...; // character we want to forget
           hf.update(out,c); // update hash value
           hf.hashvalue; // at all times, this contains the hash value
        }

##  Requirements 

GNU GCC.

##  What should I do after I download it?


type:

        make

then

        ./unit

then

        ./speedtesting




##  Reference 

Daniel Lemire, Owen Kaser: Recursive n-gram hashing is pairwise independent, at best, Computer Speech & Language, Volume 24, Issue 4, October 2010, Pages 698-710 http://arxiv.org/abs/0705.4676
