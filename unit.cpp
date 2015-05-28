#include <map>
#include <deque>

#include "cyclichash.h"
#include "rabinkarphash.h"
#include "generalhash.h"


#include "threewisehash.h"

using namespace std;





template<class hashfunction>
bool isItAFunction(uint L = 7) {
	mersenneRNG generator(5);
	const uint n(3);//n-grams
	hashfunction hf(n,L );
	deque<unsigned char> s;
	for(uint32 k = 0; k<n;++k) {
	  unsigned char c = static_cast<unsigned char>(generator()+65);
	  s.push_back(c);
	  hf.eat(c);
	}
	for(uint32 k = 0; k<100000;++k) {
		unsigned char out = s.front();
		 s.pop_front();
		char c (generator()+65);

		s.push_back(c);
		hf.update(out,c);
		if(hf.hash(s) != hf.hashvalue) {
			for(deque<unsigned char>::iterator ii=s.begin(); ii!=s.end(); ++ii)
			cout<<*ii<<" "<<static_cast<uint32>(*ii)<<endl;
		  	cerr<<"bug"<<endl;
		  	cerr<<s[0]<<s[1]<<s[2]<<" was hashed to "<<hf.hashvalue
		  	<<" when true hash value is "<<hf.hash(s)<<endl;
		  	for(uint j = 0; j<n;++j)
		  	  cerr<<s[j]<<"->"<<hf.hasher.hashvalues[s[j]]<<endl;
		  	return false;
		}
	} 
	return true;
}


template<class hashfunction>
bool doesReverseUpdateWorks(uint L = 7) {
	mersenneRNG generator(5);
	const uint n(3);//n-grams
	hashfunction hf(n,L );
	deque<unsigned char> s;
	for(uint32 k = 0; k<n;++k) {
	  unsigned char c = static_cast<unsigned char>(generator()+65);
	  s.push_back(c);
	  hf.eat(c);
	}
	for(uint32 k = 0; k<100000;++k) {
		unsigned char out = s.front();
		 s.pop_front();
		char c (generator()+65);
		s.push_back(c);
		hf.update(out,c);
		hf.reverse_update(out,c);
		hf.update(out,c);
		if(hf.hash(s) != hf.hashvalue) {
		  	return false;
		}
	}
	return true;
}



template<class hashfunction>
bool isItRandom(uint L = 19) {
  cout<<"checking that it is randomized "<<endl;
  int n = 5;
  vector<unsigned char> data(n);
  for(int k =  0; k < n; ++k ) {
    data[k] = static_cast<unsigned char>(k);
  }
  hashfunction base(n,L );
  uint64 x = base.hash(data); 
  for(int k =  0; k < 100; ++k ) {
    	hashfunction hf(n,L);
    	uint64 y = hf.hash(data); 
    	if(y != x) {
            cout<<"It is randomized! "<<endl;
            return true;
        }
          cout<<"collision "<<y<<endl;

  }
  cout<<"Not randomized! "<<endl;
  return false;// we conclude that it always hashes to the same value (this is bad)
}


bool test() {
	bool ok(true);
	cout<<"Karp-Rabin"<<endl;
	for(uint L = 1; L<=32;++L) {
		if(!ok) return false;
		ok&=isItAFunction<KarpRabinHash<> >();
	}
	ok&=isItRandom<KarpRabinHash<> >();
	for(uint L = 1; L<=64;++L) {
		if(!ok) return false;
		ok&=isItAFunction<KarpRabinHash<uint64> >();
	}
	ok&=isItRandom<KarpRabinHash<uint64> >();
	if(!ok) return false;
	cout<<"cyclic"<<endl;
	for(uint L = 2; L<=32;++L) {
	  if(!ok) return false;
 	  ok&=isItAFunction<CyclicHash<> >(L);
 	  ok&=doesReverseUpdateWorks<CyclicHash<> >(L);
	}
	for(uint L = 2; L<=64;++L) { 
	  if(!ok) return false;
 	  ok&=isItAFunction<CyclicHash<uint64> >(L);
	}
	ok&=isItRandom<CyclicHash<> >();
	ok&=isItRandom<CyclicHash<uint64> >();

    cout<<"three-wise"<<endl;
	for(uint L = 1; L<=32;++L) {
	  ok&=isItAFunction<ThreeWiseHash<> >(L);
	}
	ok&=isItRandom<ThreeWiseHash<> >();
	for(uint L = 1; L<=64;++L) {
	  ok&=isItAFunction<ThreeWiseHash<uint64> >(L);
	}
	ok&=isItRandom<ThreeWiseHash<uint64> >();

	cout<<"general"<<endl;
	ok&=isItAFunction<GeneralHash<NOPRECOMP> >(9);
	if(!ok) return false;
	ok&=isItRandom<GeneralHash<NOPRECOMP> >();
	if(!ok) return false;
	ok&=isItAFunction<GeneralHash<NOPRECOMP> >(19);
	cout<<"general"<<endl;
	ok&=isItAFunction<GeneralHash<FULLPRECOMP> >(9);
	if(!ok) return false;
	ok&=isItRandom<GeneralHash<FULLPRECOMP> >();
	if(!ok) return false;
	ok&=isItAFunction<GeneralHash<FULLPRECOMP> >(19);	
	return ok;
}


int main() {
	bool ok(test());
	if(ok)
	cout<<"your code is ok!"<<endl;
	else
	cout<<"you have a bug of some kind"<<endl;
	return 0;
}

