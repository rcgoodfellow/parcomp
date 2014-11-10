#include "prefix.hxx"
#include <iostream>

using namespace par;

PrefixNode::PrefixNode(mpi::Communicator comm, int N, int a, int b, int seed)
  : comm{comm}, N{N}, n{(int)ceil(N/(double)comm.size)}, seed{seed}, x0{a,0,b,1}
{}
  
void par::setModulo(int m) {
  cmod = m;
}


CMat
par::operator*(const CMat &a, const CMat &b) {
  return {(a[0]*b[0] + a[1]*b[2])%cmod , (a[0]*b[1] + a[1]*b[3])%cmod,
          (a[2]*b[0] + a[3]*b[2])%cmod , (a[2]*b[1] + a[3]*b[3])%cmod};
}

CMat
par::operator*(const CVec &v, const CMat &m) {
  return {(v[0]*m[0] + v[1]*m[2])%cmod, (v[0]*m[1] + v[1]*m[3])%cmod}; 
}

void
PrefixNode::run()
{
  runLocal();
  runGlobal();
  updateLocal();
}

void
PrefixNode::runLocal() {
  int start = comm.rank * n + 1,
      end = std::min(start + (n-1), N);
  if(start > N) return;
  xs.push_back(x0);
  for(int i=start; i<end; ++i) xs.push_back(xs.back() * x0);
}

void
PrefixNode::runGlobal() {
  CMat total;
  if(!xs.empty()) total = xs.back();
  else total = {0,0,0,0};
  int rounds = ceil(log2f(comm.size));
  for(int i=0; i<rounds; ++i) {
    int tgt = mpi::flipRank(comm.rank, i);
    if(tgt >= comm.size) continue;
    mpi::send(total, tgt, PREFIX_TAG);
    CMat recvd = mpi::recv<4>(tgt, PREFIX_TAG);
    total = total * recvd;
    if(tgt < comm.rank) partial = partial * recvd;
  }
}

void
PrefixNode::updateLocal() {
  for(CMat &m : xs) m = m * partial;
  const CVec v{seed, 1};
  for(const CMat &m : xs) {
    int val = (v*m)[0];
    result.push_back(val);
  }
}
