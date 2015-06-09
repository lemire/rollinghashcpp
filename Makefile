#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

CXXFLAGS = -ggdb -g3 -O2  -Wall  -Woverloaded-virtual  -Wsign-promo -Wold-style-cast 
#-DNDEBUG
all: unit speedtesting example example2 example3 example64bits

SRCS = unit.cpp speedtesting.cpp example.cpp example2.cpp example3.cpp example64bits.cpp

package:
	zip -9 ngramhashing_`date +%Y-%m-%d`.zip Makefile README *.h *.cpp

depend:
	makedepend -- $(CXXFLAGS) -- $(SRCS)

clean:
	rm -f *.o unit speedtesting example example2 example64bits

HEADERS=cyclichash.h characterhash.h mersennetwister.h rabinkarphash.h generalhash.h threewisehash.h
unit.o: $HEADERS
speedtesting.o: $HEADERS
example.o: $HEADERS
example2.o: $HEADERS
example3.o: $HEADERS
example64bits.o: $HEADERS
