
set -x
set -e

cd build

cmake -S .. -B . -D CMAKE_INSTALL_PREFIX=$1 -D CMAKE_C_COMPILER=/home/lab/bin/mpich/bin/mpicc -D CMAKE_CXX_COMPILER=/home/lab/bin/mpich/bin/mpic++

cmake --build . -j

cmake --install .