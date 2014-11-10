#ifndef PAR_SSORT
#define PAR_SSORT

#include "mpi.hxx"
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

namespace par {

class SsortNode {

  public:
    SsortNode(mpi::Communicator comm, int N, std::vector<int> A);

    void run();
    void sortLocal();
    void localPivots();
    void globalPivots();
    void chunkify();
    
    std::vector<int> S;

  private:
    mpi::Communicator comm;
    int N;
    std::vector<int> A, l_pivots, g_pivots;
    std::ofstream log;

    void logVec(std::string msg, const std::vector<int> &v);


};


}

#endif
