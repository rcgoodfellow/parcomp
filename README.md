parcomp
=======

cpts483

Build
-----

Ninja
-----
mkdir build
cd build
cmake .. -G Ninja
ninja

Make
----
mkdir build
cd build
cmake ..
make

you will need to modify the mpi hosts file for your particular system. The perl scripts run prefix tests and run ssort tests will run the prefix and ssorttests.
