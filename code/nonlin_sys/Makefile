SRC=$(wildcard *.cpp)
EXE=$(addsuffix .exe, $(basename $(SRC)))
CPPC=clang++ -std=c++14 
CPPC=g++ -std=c++14 
CPPC_FLAGS=-Wall -Wextra -pedantic -O3 
DCO=/home/un110076/Software/dco
EIGEN=/home/un110076/Software/Eigen
LIN_SYS=/home/un110076/Documents/gitlab/teaching/ss18/SEP_CES/code/lin_sys_generic

all : $(EXE)

%.exe : %.cpp
	$(CPPC) $(CPPC_FLAGS) $< -I$(EIGEN) -I$(DCO) -I$(LIN_SYS) -o $@

pdf : 
	mpage -1 -H *.h *.cpp >source.ps
	ps2pdf source.ps

clean :
	rm -fr $(EXE) *.out html latex source.*

.PHONY: all clean pdf

