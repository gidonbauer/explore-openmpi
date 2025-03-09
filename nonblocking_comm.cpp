#include <print>

#include <mpi.h>

auto main(int argc, char** argv) -> int {
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Init failed: {}", std::strerror(errno));
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
  MPI_Barrier(MPI_COMM_WORLD);

  // - Round robin send-recv -----------------------------------------------------------------------
  int send = rank;
  int recv = -1;

  int dest_rank = (rank + 1) % num_rank;
  int src_rank  = (rank - 1 + num_rank) % num_rank;

  MPI_Request request;
  if (MPI_Isend(&send, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD, &request) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Isend failed: {}", std::strerror(errno));
    MPI_Finalize();
    return 1;
  }

  if (MPI_Recv(&recv, 1, MPI_INT, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
    std::println(stderr, "MPI_Recv failed: {}", std::strerror(errno));
    MPI_Finalize();
    return 1;
  }

  std::println("Rank {:2}: Send {:2}, Recv {:2}", rank, send, recv);

  MPI_Finalize();
}
