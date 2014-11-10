#ifndef PAR_MPI
#define PAR_MPI

#include <mpi.h>
#include <array>
#include <bitset>

namespace par { namespace mpi {

struct Communicator {
  int size, rank;
  MPI_Comm id;
  Communicator(MPI_Comm id = MPI_COMM_WORLD);
};

template<typename C, typename TY_TAG, typename COMM_TAG>
void
send(C &stuff, int target, TY_TAG ty_tag, COMM_TAG comm_tag, 
     MPI_Comm comm_w = MPI_COMM_WORLD)
{
  MPI_Send(stuff.data(), stuff.size(), ty_tag, target, comm_tag, comm_w); 
}

template<unsigned long S>
void
send(std::array<int,S> &stuff, int target, int comm_tag, 
     MPI_Comm comm_w = MPI_COMM_WORLD)
{
  send(stuff, target, MPI_INT, comm_tag, comm_w);
}

template<typename C, typename TY_TAG, typename COMM_TAG>
void
recv(C &stuff, int target, TY_TAG ty_tag, COMM_TAG comm_tag,
    MPI_Comm comm_w = MPI_COMM_WORLD, MPI_Status *status = MPI_STATUS_IGNORE)
{
  MPI_Recv(stuff.data(), stuff.size(), ty_tag, target, comm_tag, comm_w, 
           status);
}

template<unsigned long S>
std::array<int,S>
recv(int target, int comm_tag,
     MPI_Comm comm_w = MPI_COMM_WORLD, MPI_Status *status = MPI_STATUS_IGNORE)
{
  std::array<int,S> stuff;
  recv(stuff, target, MPI_INT, comm_tag, comm_w, status);
  return std::move(stuff);
}

int
flipRank(int rank, size_t i);

}}

#endif
