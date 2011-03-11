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
	deque<chartype> s;
	for(uint32 k = 0; k<n;++k) {
	  chartype c = static_cast<chartype>(generator()+65);
	  s.push_back(c);
	  hf.eat(c);
	}
	for(uint32 k = 0; k<100000;++k) {
		chartype out = s.front();
		 s.pop_front();
		char c (generator()+65);
		//cout<<"met char "<<c<<" "<<static_cast<uint32>(c)<<endl;
		s.push_back(c);
		hf.update(out,c);
		if(hf.hash(s) != hf.hashvalue) {
			for(deque<chartype>::iterator ii=s.begin(); ii!=s.end(); ++ii)
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

bool test() {
	bool ok(true);
	cout<<"three-wise"<<endl;
	for(uint L = 1; L<=32;++L) {
      if(!ok) return false;
	  ok&=isItAFunction<ThreeWiseHash>(L);
	}
	cout<<"general"<<endl;
	ok&=isItAFunction<GeneralHash<NOPRECOMP> >(9);
	if(!ok) return false;
	ok&=isItAFunction<GeneralHash<NOPRECOMP> >(19);
	cout<<"general"<<endl;
	ok&=isItAFunction<GeneralHash<FULLPRECOMP> >(9);
	if(!ok) return false;
	ok&=isItAFunction<GeneralHash<FULLPRECOMP> >(19);
	cout<<"cyclic"<<endl;
	for(uint L = 1; L<=32;++L) {
	  if(!ok) return false;
 	  ok&=isItAFunction<CyclicHash>(L);
	}
	cout<<"Karp-Rabin"<<endl;
	for(uint L = 1; L<=32;++L) {
		if(!ok) return false;
		ok&=isItAFunction<KarpRabinHash>();
	}
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

