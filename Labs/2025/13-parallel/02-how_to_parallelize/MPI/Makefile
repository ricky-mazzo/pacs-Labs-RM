
CXX = mpicxx
CXXFLAGS = -O2 -std=c++17

TARGET = kmc_mpi
SRC = kmc_mpi_io.cpp
DEPS = stepper.h 

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) new_parallel_kmc_presentation.pdf *.o *.csv *.out *.nav *.vrb *.snm *.toc *.aux *.log


docs:
	pdflatex -output-directory=. new_parallel_kmc_presentation.tex
