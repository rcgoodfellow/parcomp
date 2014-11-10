#!/usr/bin/perl
use strict;
use warnings;

print "Running Prefix Tests\n";
my @procs = (1..16);

for(my $i=4; $i<=24; $i+=4) {
  my $n = 2 ** $i;
  print "###>> n = 2^$i = $n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  foreach(@procs) {
    `rm -f *.log`;
    `rm -f prefix.time`;
    `mpiexec -n $_ -f mpi_hosts ./prefix_tests $n 7 19 0 9972`;
    my $data = `cat *.log`;
    my $data_head = substr $data, 0, 15;
    my $data_tail = substr $data, -15;
    printf "%-5s %s ... %s ", "[$_]", $data_head, $data_tail;
    #printf "%-5s %s", "[$_]", $data;
    printf "%s \n", `cat prefix.time`;
  }
}
