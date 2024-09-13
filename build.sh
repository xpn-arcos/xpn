
set -x
set -e

cd build

cmake -S .. -B . -DCMAKE_INSTALL_PREFIX=/home/lab/bin/xpn -DCMAKE_C_COMPILER=/home/lab/bin/mpich/bin/mpicc

cmake --build . --clean-first -j

cmake --install .