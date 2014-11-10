#include "prefix.hxx"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace par;
using std::cout;
using std::endl;
using std::ofstream;
using std::to_string;
using namespace std::chrono;

int main(int argc, char **argv){

  if(argc != 6) {
    std::cerr << "usage: prefix <N> <a> <b> <x0> <m>" << endl;
    return EXIT_FAILURE;
  }

  int N = atoi(argv[1]),
      a = atoi(argv[2]),
      b = atoi(argv[3]),
      x0 = atoi(argv[4]),
      m = atoi(argv[5]);
  

  mpi::Communicator comm;
  setModulo(m);
  PrefixNode pn(comm, N, a, b, x0); 

  auto start = high_resolution_clock::now();
  pn.run();
  MPI_Barrier(MPI_COMM_WORLD);
  auto end = high_resolution_clock::now();

  std::string rank_str = to_string(comm.rank);
  if(comm.rank < 10) { rank_str = "0" + rank_str; }
  ofstream log("prefix"+rank_str+".log");
  for(int x : pn.result) log << x << " ";
  log << std::flush;
  
  if(comm.rank == 0) {
    ofstream time("prefix.time");
    time << duration_cast<microseconds>(end-start).count()/10000.0 << std::flush;
  }
  
  MPI_Barrier(MPI_COMM_WORLD);

}
