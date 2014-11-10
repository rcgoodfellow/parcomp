#include "prefix.hxx"
#include "ssort.hxx"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace par;
using std::cout;
using std::endl;
using std::ofstream;
using std::to_string;
using namespace std::chrono;

int main(int argc, char **argv) {

  if(argc != 6) {
    std::cerr << "usage: ssort <N> <a> <b> <x0> <m>" << endl;
    return EXIT_FAILURE;
  }
  
  int N = atoi(argv[1]),
      a = atoi(argv[2]),
      b = atoi(argv[3]),
      x0 = atoi(argv[4]),
      m = atoi(argv[5]);
  

  std::cout << "Generating input ... " << std::flush;
  mpi::Communicator comm;
  setModulo(m);
  PrefixNode pn(comm, N, a, b, x0); 
  pn.run();
  MPI_Barrier(MPI_COMM_WORLD);
  std::cout << "Done" << std::endl;

  auto start = high_resolution_clock::now();
  SsortNode sn(comm, N, pn.result);
  sn.run();
  MPI_Barrier(MPI_COMM_WORLD);
  auto end = high_resolution_clock::now();

  std::string rank_str = to_string(comm.rank);
  if(comm.rank < 10) { rank_str = "0" + rank_str; }
  ofstream log("ssort"+rank_str+".log");
  for(int x : sn.S) log << x << " ";
  log << std::flush;

  if(comm.rank == 0) {
    ofstream time("ssort.time");
    time << duration_cast<microseconds>(end-start).count()/1000.0
         << " " << sn.comtime << std::flush;
  }
  MPI_Barrier(MPI_COMM_WORLD);

}
