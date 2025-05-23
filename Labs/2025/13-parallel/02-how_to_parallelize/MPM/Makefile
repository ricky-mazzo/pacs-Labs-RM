
## EXMPLE OMP FLAGS :
## CPPFLAGS = "-I./quadgrid/include -I./json/single_include/nlohmann/ -I./json/single_include-O3 -I/opt/local/include/ "
## CXXFLAGS = "-O3 -fopenmp"
## LDFLAGS = " -L/opt/local/lib/libomp "

## EXMPLE PSTL via oneDPC++ FLAGS :
## CPPFLAGS = "-DONEDPL_USE_TBB_BACKEND -I/opt/local/libexec/onetbb/include -I/opt/local/include/ prova_omp.cpp "
## CXXFLAGS = "-O3 "
## LDFLAGS = "-L/opt/local/libexec/onetbb/lib/"
## LIBS = "-ltbb"

CXX ?= /opt/local/bin/clang++
CPPFLAGS ?= -I./quadgrid/include -I./json/single_include/nlohmann/ -I./json/single_include
CXXFLAGS ?= -std=c++17
QUADGRIDSRC ?= ./quadgrid/src/particles.cpp
LDFLAGS ?=  -L/opt/local/lib
LIBS ?= 

grid_velocity_field : grid_velocity_field.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(QUADGRIDSRC) grid_velocity_field.cpp -o grid_velocity_field $(LDFLAGS) $(LIBS)


loaddeps :
	git clone --depth 1 --branch reverse_loop git@github.com:carlodefalco/quadgrid.git
	git clone --depth 1 git@github.com:nlohmann/json.git

clean :
	$(RM) *.o *.csv *.vts *.json grid_velocity_field
