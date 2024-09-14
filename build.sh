
set -x
set -e

cd build

cmake -S .. -B . -D CMAKE_INSTALL_PREFIX=/home/lab/bin/xpn -D CMAKE_C_COMPILER=/home/lab/bin/mpich/bin/mpicc -D CMAKE_CXX_COMPILER=/home/lab/bin/mpich/bin/mpic++

cmake --build . -v -j 4

cmake --install .