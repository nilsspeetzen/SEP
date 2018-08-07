SRC=$(wildcard *.cpp)

EXE=$(addsuffix .exe, $(basename $(SRC)))

CPPC=clang++ -std=c++17

CPPC=g++-8 -std=c++17

#CPPC_FLAGS=-Wall -Wextra -pedantic -O3

OMP=-fopenmp
DCO=../../../lib/dco
EIGEN=../../../lib/eigen
LINSYS=../lin_sys
NONLINSYS=../nonlin_sys


all : $(EXE)



%.exe : %.cpp

	$(CPPC) $(CPPC_FLAGS) $(OMP) $< -I$(EIGEN) -I$(DCO) -I$(LINSYS) -I$(NONLINSYS) -o $@



pdf :
	mpage -1 -H *.h *.cpp >source.ps

	ps2pdf source.ps



clean :
	rm -fr $(EXE) *.out html latex source.*



.PHONY: all clean pdf
