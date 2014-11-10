#ifndef PAR_PREFIX
#define PAR_PREFIX

#include "mpi.hxx"
#include <array>
#include <string>
#include <vector>
#include <cmath>
#include <bitset>

namespace par {

  //congruential matrix
  using CMat = std::array<int, 4>;
  using CVec = std::array<int, 2>;

  //congruentual modulo
  int cmod{1};

  void setModulo(int m);

  CMat operator*(const CMat &a, const CMat &b);
  CMat operator*(const CVec &v, const CMat &m);

  const int PREFIX_TAG{
    static_cast<int>(std::hash<std::string>{}("PAR_PREFIX_TAG"))};

  class PrefixNode {
    public:
      PrefixNode(mpi::Communicator comm, int N, int a, int b, int seed=0);
      void run();
      void runLocal();
      void runGlobal();
      void updateLocal();
      
      std::vector<int> result;

    private:
      mpi::Communicator comm;
      int N, //global input size 
          n, //local input size
          seed;
      CMat x0, //starting congruential matrix
           partial{1,0,0,1}; //partial multiplier
      std::vector<CMat> xs; //congruential matricies computed by this node
  };

}

#endif
