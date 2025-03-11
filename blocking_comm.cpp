#include <print>

#include <mpi.h>

auto main(int argc, char** argv) -> int {
  MPI_Init(&argc, &argv);

  int rank, num_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_rank);

  if (rank == 0) {
    int version, subversion;
    MPI_Get_version(&version, &subversion);
    std::println("MPI version: {}.{}", version, subversion);

    std::println("{} ranks in MPI_COMM_WORLD", num_rank);
  }

  // - Round robin send-recv -----------------------------------------------------------------------
  int send = rank + 1 < num_rank ? rank : -1;
  int recv = -1;
  if (rank > 0) { MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); }

  std::println("Rank {:2}: Send {:2}, Recv {:2}", rank, send, recv);

  if (rank + 1 < num_rank) { MPI_Send(&send, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); }

  MPI_Finalize();
}
