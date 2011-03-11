#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

CXXFLAGS = -g0 -O3  -Wall  -Woverloaded-virtual  -Wsign-promo -Wold-style-cast 
#-DNDEBUG
all: unit speedtesting

package:
	zip -9 ngramhashing_`date +%Y-%m-%d`.zip Makefile README *.h *.cpp

clean:
	rm -f *.o unit speedtesting
