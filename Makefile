TARGETS = ${basename ${wildcard *.cpp}}
CXX_FLAGS = -Wall -Wextra -pedantic -Wshadow -Wconversion -march=native -std=c++23 -g
OMPI_INC = ${shell pkg-config --cflags ompi}
OMPI_LIBS = ${shell pkg-config --libs ompi}

all: CXX_FLAGS += -O3
all: ${TARGETS}

debug: CXX_FLAGS += -fsanitize=address,undefined,leak -O0
debug: ${TARGETS}

%: %.cpp
	${CXX} ${CXX_FLAGS} ${OMPI_INC} -o $@ $< ${OMPI_LIBS}

clean:
	${RM} -r ${TARGETS} ${addsuffix .dSYM, ${TARGETS}}
