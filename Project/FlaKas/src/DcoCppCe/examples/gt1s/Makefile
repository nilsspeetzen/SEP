CXX=/usr/bin/c++
CPPFLAGS=-O3 -DNDEBUG

main: main.o driver.o
	$(CXX) -o main.exe $^

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) -o $@ $<

clean:
	rm main.o driver.o main.exe
