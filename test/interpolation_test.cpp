#include <gtest/gtest.h>
#include <points2grid/Interpolation.hpp>

#include <points2grid/config.h>
#include <points2grid/Global.hpp>

#include "fixtures.hpp"


namespace points2grid
{


namespace
{


struct AscHeader
{
    int ncols;
    int nrows;
    double xllcorner;
    double yllcorner;
    double cellsize;
    int NODATA_value;
};

struct GridHeader
{
    double north;
    double south;
    double east;
    double west;
    int rows;
    int cols;
};


AscHeader read_asc_header(std::istream& is)
{
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

GridHeader read_grid_header(std::istream& is)
{
    GridHeader header;
    std::streamsize n = std::numeric_limits<std::streamsize>::max();
    is.ignore(n, ' ');
    is >> header.north;
    is.ignore(n, ' ');
    is >> header.south;
    is.ignore(n, ' ');
    is >> header.east;
    is.ignore(n, ' ');
    is >> header.west;
    is.ignore(n, ' ');
    is >> header.rows;
    is.ignore(n, ' ');
    is >> header.cols;
    return header;
}


class InterpolationTest : public FourPointsTest
{};


}


TEST_F(InterpolationTest, Constructor)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
}


TEST_F(InterpolationTest, InitAscii)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    int retval = interp.init(infile, INPUT_ASCII);
    EXPECT_EQ(0, retval);
}


TEST_F(InterpolationTest, Interpolate)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_ASCII);
    int retval = interp.interpolation(infile, outfile, INPUT_ASCII, OUTPUT_FORMAT_ARC_ASCII, OUTPUT_TYPE_ALL);
    EXPECT_EQ(0, retval);
    EXPECT_EQ(4, interp.getDataCount());
    EXPECT_EQ(2, interp.getGridSizeX());
    EXPECT_EQ(2, interp.getGridSizeY());
}


TEST_F(InterpolationTest, ArcGISHeaders)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_ASCII);
    interp.interpolation(infile, outfile, INPUT_ASCII, OUTPUT_FORMAT_ALL, OUTPUT_TYPE_ALL);

    // Test ArcGIS headers
    std::ifstream asc;
    asc.open((outfile + ".idw.asc").c_str());
    AscHeader asc_header = read_asc_header(asc);
    EXPECT_EQ(asc_header.ncols, 2);
    EXPECT_EQ(asc_header.nrows, 2);
    EXPECT_DOUBLE_EQ(asc_header.xllcorner, 0.5);
    EXPECT_DOUBLE_EQ(asc_header.yllcorner, 0.5);
    EXPECT_DOUBLE_EQ(asc_header.cellsize, 1.0);
    EXPECT_EQ(asc_header.NODATA_value, -9999);
}


TEST_F(InterpolationTest, GridHeaders)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_ASCII);
    interp.interpolation(infile, outfile, INPUT_ASCII, OUTPUT_FORMAT_ALL, OUTPUT_TYPE_ALL);

    // Test GRID headers
    std::ifstream grid;
    grid.open((outfile + ".idw.grid").c_str());
    GridHeader grid_header = read_grid_header(grid);
    EXPECT_DOUBLE_EQ(grid_header.north, 2.5);
    EXPECT_DOUBLE_EQ(grid_header.south, 0.5);
    EXPECT_DOUBLE_EQ(grid_header.east, 2.5);
    EXPECT_DOUBLE_EQ(grid_header.west, 0.5);
    EXPECT_EQ(grid_header.rows, 2);
    EXPECT_EQ(grid_header.cols, 2);
}


}
