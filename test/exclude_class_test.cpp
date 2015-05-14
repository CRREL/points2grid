#include <gtest/gtest.h>
#include <points2grid/Interpolation.hpp>

#include <points2grid/config.h>
#include <points2grid/Global.hpp>
#include <cmath>

#include "fixtures.hpp"


namespace points2grid {


namespace {

    class ExcludeClassTest : public ExcludePointsTest {
    };

}

struct AscHeader {
    int ncols;
    int nrows;
    double xllcorner;
    double yllcorner;
    double cellsize;
    int NODATA_value;
};

struct AsciiData {
    AsciiData(int numvals) : values(0) {
        if (numvals > 0) values = new double[numvals];
    }

    ~AsciiData() {
        if (values) delete[] values;
    }

    double *values;
};

AscHeader read_asc_header(std::istream &is) {
    AscHeader header;
    std::streamsize n = std::numeric_limits<std::streamsize>::max();
    is.ignore(n, ' ');
    is >> header.ncols;
    is.ignore(n, ' ');
    is >> header.nrows;
    is.ignore(n, ' ');
    is >> header.xllcorner;
    is.ignore(n, ' ');
    is >> header.yllcorner;
    is.ignore(n, ' ');
    is >> header.cellsize;
    is.ignore(n, ' ');
    is >> header.NODATA_value;
    return header;
}

AsciiData read_asc_data(std::istream &is) {
    AscHeader hdr = read_asc_header(is);
    AsciiData data(hdr.ncols * hdr.nrows);

    for (int i = 0; i < hdr.ncols * hdr.nrows; ++i)
        is >> data.values[i];

    return data;
}

TEST_F(ExcludeClassTest, InitLAS) {
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    int retval = interp.init(infile, INPUT_LAS);
    EXPECT_EQ(0, retval);
}


TEST_F(ExcludeClassTest, Interpolate) {
    std::vector<int> exclude_class;
    exclude_class.push_back(1);

    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_LAS);
    interp.setLasExcludeClassification(exclude_class);

    int retval = interp.interpolation(infile, outfile, INPUT_LAS, OUTPUT_FORMAT_ARC_ASCII, OUTPUT_TYPE_ALL);
    EXPECT_EQ(0, retval);

    // Get a count of valid data
    std::ifstream asc;
    asc.open((outfile + ".idw.asc").c_str());
    AscHeader asc_header = read_asc_header(asc);
    AsciiData ascdata = read_asc_data(asc);

    int valid_data = 0;

    for (int i = 0; i < asc_header.ncols * asc_header.nrows; ++i) {
        if (ascdata.values[i] != asc_header.NODATA_value) {
            ++valid_data;
        }
    }

    EXPECT_EQ(4056, valid_data);
}

}