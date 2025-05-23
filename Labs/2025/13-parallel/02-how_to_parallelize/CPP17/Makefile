
CXX=clang++
CXXFLAGS=-O3 -std=gnu++17

all: kmc

kmc_mpi_gather: kmc.cpp
	$(CXX) $(CXXFLAGS) -o kmc kmc.cpp

clean:
	rm -f kmc kmc_presentation.pdf *.o *.csv *.out *.nav *.vrb *.snm *.toc *.aux *.log

docs: kmc_presentation.pdf

kmc_presentation.pdf: kmc_presentation.tex
	pdflatex -shell-escape kmc_presentation.tex
	pdflatex -shell-escape kmc_presentation.tex
