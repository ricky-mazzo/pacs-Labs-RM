
CXX=mpicxx
CXXFLAGS=-O3 -std=c++17

all: kmc_mpi_gather

kmc_mpi_gather: kmc_mpi_gather.cpp
	$(CXX) $(CXXFLAGS) -o kmc_mpi_gather kmc_mpi_gather.cpp

clean:
	rm -f kmc_mpi_gather kmc_mpi_gather_presentation.pdf *.o *.csv *.out *.nav *.vrb *.snm *.toc *.aux *.log

run:
	mpirun -np 4 ./kmc_mpi_gather

docs: kmc_mpi_gather_presentation.pdf

kmc_mpi_gather_presentation.pdf: kmc_mpi_gather_presentation.tex
	pdflatex -shell-escape kmc_mpi_gather_presentation.tex
	pdflatex -shell-escape kmc_mpi_gather_presentation.tex
