if [[ "$POINTS2GRID_GDAL" == "ON" ]]; then
    WITH_GDAL=ON
else
    WITH_GDAL=OFF
fi

mkdir build && cd build
cmake .. -DWITH_TESTS=ON -DCMAKE_BUILD_TYPE=Debug -DWITH_GDAL=$WITH_GDAL && \
    make && \
    test/points2grid-test && \
    sudo make install
