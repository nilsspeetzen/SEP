SRC=$(wildcard *.cpp)

EXE=$(addsuffix .exe, $(basename $(SRC)))

CPPC=clang++ -std=c++14

CPPC=g++-8 -std=c++14

#CPPC_FLAGS=-Wall -Wextra -pedantic -O3
CPPC_FLAGS=
DCO=../../../lib/dco
/dcw6i32nel/includeasd
EIGEN=../../../lib/eigen


all : $(EXE)



%.exe : %.cpp
	$(CPPC) $(CPPC_FLAGS) $< -I$(EIGEN) -I$(DCO) -o $@


pdf :
	mpage -1 -H *.h *.cpp >source.ps

	ps2pdf source.ps


clean :

	rm -fr $(EXE) *.out html latex source.*



.PHONY: all clean pdf

