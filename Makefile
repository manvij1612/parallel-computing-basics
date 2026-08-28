# Build the MPI and OpenMP exercises.
#
#   make            # build everything
#   make omp        # build only the OpenMP examples
#   make mpi        # build only the MPI examples
#   make clean
#
# On macOS, install an OpenMP-capable toolchain first, e.g.:
#   brew install libomp open-mpi
# and, if using Apple clang, set:
#   make OMP_FLAGS="-Xpreprocessor -fopenmp -lomp"

CC       ?= gcc
MPICC    ?= mpicc
OMP_FLAGS ?= -fopenmp
CFLAGS   ?= -O2 -Wall -std=c11
LDLIBS   ?= -lm

OMP_SRC  := $(wildcard omp_stuff/*.c)
OMP_BIN  := $(OMP_SRC:.c=)

# myCode.c needs secret_function.h / a secret_function implementation that is
# not distributed with the repo, so it is excluded from the default build.
MPI_SRC  := $(filter-out mpi_stuff/myCode.c, $(wildcard mpi_stuff/*.c))
MPI_BIN  := $(MPI_SRC:.c=)

.PHONY: all omp mpi clean
all: omp mpi
omp: $(OMP_BIN)
mpi: $(MPI_BIN)

omp_stuff/%: omp_stuff/%.c
	$(CC) $(CFLAGS) $(OMP_FLAGS) -o $@ $< $(LDLIBS)

mpi_stuff/%: mpi_stuff/%.c
	$(MPICC) $(CFLAGS) $(OMP_FLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(OMP_BIN) $(MPI_BIN) mpi_stuff/myCode
