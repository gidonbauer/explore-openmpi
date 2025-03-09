#include <print>

#include <mpi.h>

auto main(int argc, char** argv) -> int {
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Init failed: {}", std::strerror(errno));
    return 1;
  }

  int rank;
  if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Comm_rank failed: {}", std::strerror(errno));
    MPI_Finalize();
    return 1;
  }

  int num_rank;
  if (MPI_Comm_size(MPI_COMM_WORLD, &num_rank) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Comm_size failed: {}", std::strerror(errno));
    MPI_Finalize();
    return 1;
  }

  if (rank == 0) {
    int version, subversion;
    MPI_Get_version(&version, &subversion);
    std::println("MPI version: {}.{}", version, subversion);

    std::println("{} ranks in MPI_COMM_WORLD", num_rank);
  }

  // - Round robin send-recv -----------------------------------------------------------------------
  int send = rank + 1 < num_rank ? rank : -1;
  int recv = -1;
  if (rank > 0) {
    if (MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) !=
        MPI_SUCCESS) {
      std::println(stderr, "MPI_Recv failed: {}", std::strerror(errno));
      MPI_Finalize();
      return 1;
    }
  }

  std::println("Rank {:2}: Send {:2}, Recv {:2}", rank, send, recv);

  if (rank + 1 < num_rank) {
    if (MPI_Send(&send, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      std::println(stderr, "MPI_Send failed: {}", std::strerror(errno));
      MPI_Finalize();
      return 1;
    }
  }

  MPI_Finalize();
}
