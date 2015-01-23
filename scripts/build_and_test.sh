mkdir build && cd build
cmake .. -DWITH_TESTS=ON -DCMAKE_BUILD_TYPE=Debug && \
    make && \
    test/points2grid-test && \
    sudo make install
