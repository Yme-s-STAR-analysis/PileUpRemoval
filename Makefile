all: run1 run2 run3

run1: src1.cxx CentCorrTool/CentCorrTool.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 -O3

run2: src2.cxx CentCorrTool/CentCorrTool.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 -O3
	
run2X: src2X.cxx CentCorrTool/CentCorrTool.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 -O3

run3: src3.cxx CentCorrTool/CentCorrTool.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++11 -O3

fit1: fit1.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++17 -O3

fit2: fit2.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++17 -O3

plot: plot.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++17 -O3

clean:
	@rm run[12] fit[12] *pdf
