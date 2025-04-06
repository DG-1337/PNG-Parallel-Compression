CXX = g++
CXXFLAGS = -std=c++11 -stdlib=libc++ -w -Weverything -lz

IMG_DIR = 

# Executables
TARGETS = parallel-compression create-tests create-img-dir

# Source files
PARALLEL_SRC = src/main.cpp src/lodepng.cpp src/compression/compression.cpp src/filter/adaptive_filter.cpp src/filter/read_scan_line.cpp src/filter/filter.cpp
TESTS_SRC    = tests/create_pngs.cpp src/lodepng.cpp

# Object files
PARALLEL_OBJ = $(PARALLEL_SRC:.cpp=.o)
TESTS_OBJ    = $(TESTS_SRC:.cpp=.o)

all: create-img-dir $(TARGETS)

parallel-compression: $(PARALLEL_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

create-tests: $(TESTS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

create-img-dir: 
	@mkdir -p tests/img

# Compile .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean: 
	rm -f src/*.o tests/*.o *.o $(TARGETS) 
	rm -f *.png tests/img/*.png

