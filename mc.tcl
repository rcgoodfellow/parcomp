set ns [new Simulator]
source tb_compat.tcl

set NODES 8
set lanstr ""

for {set i 0} {$i < $NODES} {incr i} {
  set node($i) [$ns node]
  append lanstr "$node($i) "
  tb-set-hardware $node($i) MicroCloud
  tb-set-node-os $node($i) Ubuntu1404-64-STD
  tb-set-node-startcmd $node($i) "/proj/GridStat/compute/install_mpi.sh >% /tmp/install_mpi.log"
}

set lan [$ns make-lan $lanstr 1Gb 0ms]

$ns rtproto Static
$ns run

