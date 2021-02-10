
./install_deps.sh

./do_cmake.sh
cd bild
make -j$(nproc)
make install -j$(nproc)
