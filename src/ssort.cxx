#include "ssort.hxx"

using namespace par;
using namespace std::chrono;
  
SsortNode::SsortNode(mpi::Communicator comm, int N, std::vector<int> A)
  : comm{comm}, N{N}, A{A}
{
  log.open("_ssort_"+std::to_string(comm.rank)+".plog");
  log << "node " << comm.rank << " log" << std::endl;
}

void
SsortNode::logVec(std::string msg, const std::vector<int> &v) {
  log << msg << std::endl;
  for(const int x : v) log << x << " ";
  log << std::endl;
}

void
SsortNode::sortLocal()
{
  std::sort(A.begin(), A.end());
  logVec("input:", A);
}

void
SsortNode::localPivots()
{
  int partitions = 1;
  if(comm.size >= 2) partitions = comm.size;
  int stride = A.size() / partitions;

  for(int i=stride; i<A.size(); i+=stride) l_pivots.push_back(A[i]);

  logVec("l_pivot:", l_pivots);
}

void
SsortNode::globalPivots()
{
  int p = comm.size;
  std::vector<int> t_pivots(p*(p-1), 0);
  auto start = high_resolution_clock::now();
  MPI_Allgather(l_pivots.data(), p-1, MPI_INT, t_pivots.data(), p-1, MPI_INT, 
                MPI_COMM_WORLD);
  auto end = high_resolution_clock::now();
  comtime += duration_cast<microseconds>(end-start).count()/1000.0;
  sort(t_pivots.begin(), t_pivots.end());
  
  int partitions = 1;
  if(comm.size >= 2) partitions = comm.size;
  int stride = t_pivots.size() / partitions;

  for(int i=stride; i<t_pivots.size(); i+=stride) g_pivots.push_back(t_pivots[i]);

  logVec("g_pivot:", g_pivots);
}

void
SsortNode::chunkify()
{
  int i{0}, p{comm.size};
  std::vector<int> sendcounts(p,0);
  std::vector<int> chunks;
  //size_t _n_ = A.size();
  for(int x : A)
  {
    while(x > g_pivots[i] && i<(p-1))
    {
      chunks.push_back(-1);
      sendcounts[i++]++;
    }
    chunks.push_back(x);
    sendcounts[i]++;
  }
  while(i < p) {
    chunks.push_back(-1);
    sendcounts[i++]++;
  }

  std::vector<int> sdis(p,0);
  for(int i=1; i<p; ++i) sdis[i] = sendcounts[i-1] + sdis[i-1];

  std::vector<int> rcv(p*A.size(), -1);
  std::vector<int> rcv_cnts(p, A.size());
  std::vector<int> rcv_dspl(p, 0);
  for(int i=1; i<p; ++i) rcv_dspl[i] = rcv_cnts[i-1] + rcv_dspl[i-1];

  auto start = high_resolution_clock::now();
  MPI_Alltoallv(chunks.data(), sendcounts.data(), sdis.data(), MPI_INT,
      rcv.data(), rcv_cnts.data(), rcv_dspl.data(), MPI_INT, MPI_COMM_WORLD);
  auto end = high_resolution_clock::now();
  comtime += duration_cast<microseconds>(end-start).count()/1000.0;

  for(int x : rcv)
  {
    if(x > -1) S.push_back(x);
  }

  std::sort(S.begin(), S.end());

  logVec("result:", S);

}

void
SsortNode::run()
{
  sortLocal();
  if(comm.size == 1) {
    S = A;
    return;
  }
  localPivots();
  globalPivots();
  chunkify();
}

