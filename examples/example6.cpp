/**
* This example is from Dmitry Artamonov, it shows that to get the same
* hash values for the same substrings, you need to use the same hasher object
* (since they are randomized).
*/

#include <string>
#include <memory>
#include <cassert>
#include <iostream>
#include "cyclichash.h"
#include "generalhash.h"
#include "threewisehash.h"
#include "rabinkarphash.h"
#include "adler32.h"

void CalcHashes( const std::string& Inp, const int WindowSize, KarpRabinHash<> & h1, ThreeWiseHash<> & h2, GeneralHash<> & h3, CyclicHash<> & h4, Adler32& h5 ) {


    int WindowPos = 0;
    h1.reset();
    h2.reset();
    h3.reset();
    h4.reset();
    h5.reset();

    for (int i = 0; i < Inp.length(); i++) {
        unsigned char InChar = Inp[i];

        bool Eat = (i < WindowSize);
        unsigned char OutChar = ' ';
        if (Eat) {
            h1.eat(InChar);
            h2.eat(InChar);
            h3.eat(InChar);
            h4.eat(InChar);
            h5.eat(InChar);
        } else {
            OutChar = Inp[i - WindowSize];
            h1.update(OutChar, InChar);
            h2.update(OutChar, InChar);
            h3.update(OutChar, InChar);
            h4.update(OutChar, InChar);
            h5.update(OutChar, InChar);
        }
        if(i + 1 >= WindowSize) {
          auto current = Inp.substr(i + 1 - WindowSize, WindowSize);
          printf("%04d %02d %c %c %06x %06x %06x %06x %06x %c %s \n", i, WindowPos, InChar, OutChar, h1.hashvalue, h2.hashvalue, h3.hashvalue, h4.hashvalue,  h5.hashvalue, (Eat) ? '*' : ' ', current.c_str());
          assert(h1.hash(current) == h1.hashvalue);
          assert(h2.hash(current) == h2.hashvalue);
          assert(h3.hash(current) == h3.hashvalue);
          assert(h4.hash(current) == h4.hashvalue);

        }

        WindowPos = (WindowPos + 1) % WindowSize;
    }
}

// ----------------------------------------------------------------------------

void Compare() {
    const int WindowSize = 16;
    KarpRabinHash<> h1(WindowSize);
    ThreeWiseHash<> h2(WindowSize);
    GeneralHash<> h3(WindowSize);
    CyclicHash<> h4(WindowSize);
    Adler32 h5(WindowSize);

    std::string s1 = "Test string for rolling hashes."; // 32 chars
    CalcHashes(s1, WindowSize, h1, h2, h3, h4, h5);

    printf("---------------------------------------\n");

    std::string s2 = "This is some preamble.";
    CalcHashes(s2 + s1, WindowSize, h1, h2, h3, h4, h5);
}

int main() {
  Compare();
}
