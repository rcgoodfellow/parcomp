#!/usr/bin/perl
use strict;
use warnings;

print "Running Prefix Tests\n";

for(my $i=12; $i<=28; $i+=4) {
  my $n = 2 ** $i;
  print "###>> n = 2^$i = $n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  for(my $j=1; $j<=64; $j*=2) {
    `rm -f *.log`;
    `rm -f *.plog`;
    `rm -f *.time`;
    `mpiexec -n $j -f mpi_hosts ./ssort_tests $n 7 19 0 9972`;
    my $data = `cat *.log`;
    my $data_head = substr $data, 0, 15;
    my $data_tail = substr $data, -15;
    printf "%-5s %s ... %s ", "[$j]", $data_head, $data_tail;
    #printf "%-5s %s", "[$j]", $data;
    printf "%s \n", `cat ssort.time`;
  }
}

