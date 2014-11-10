#include "mpi.hxx"

using namespace par;
using namespace par::mpi;

Communicator::Communicator(MPI_Comm id)
  : id{id} {
    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(id, &size);
    MPI_Comm_rank(id, &rank);
}

int
par::mpi::flipRank(int rank, size_t i)
{
  std::bitset<sizeof(unsigned)*8> target(static_cast<unsigned>(rank));
  target.flip(i);
  return target.to_ulong();
}
