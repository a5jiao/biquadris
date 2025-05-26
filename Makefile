########## Variables ##########

CXX = g++-11					# compiler
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD			# compiler flags
LDFLAGS = -lX11
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

# Find all source files (*.cc) in the project directory recursively
SOURCES = $(shell find . -type f -name '*.cc')

# Automatically generate object files and dependency files
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC = biquadris


########## Targets ##########

.PHONY : clean

# Build the executable
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS}  $^ ${LDFLAGS} -o $@ 

# Include dependency files
-include ${DEPENDS}

# Clean up generated files
clean :
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}
