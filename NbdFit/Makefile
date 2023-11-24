all: fit

fit: NbdFitCore.cxx DoMyFit.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++17 -O3