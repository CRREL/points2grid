#include <gtest/gtest.h>
#include <points2grid/Interpolation.hpp>

#include <points2grid/Global.hpp>

#include "config.hpp"


namespace points2grid
{


class InterpolationTest : public ::testing::Test
{
public:

    virtual void SetUp()
    {
        infile = get_test_data_filename("four-points.txt");
        outfile = get_test_data_filename("outfile");
    }

    virtual void TearDown()
    {
        std::remove((outfile + ".den.asc").c_str());
        std::remove((outfile + ".idw.asc").c_str());
        std::remove((outfile + ".max.asc").c_str());
        std::remove((outfile + ".mean.asc").c_str());
        std::remove((outfile + ".min.asc").c_str());
        std::remove((outfile + ".std.asc").c_str());
    }

    std::string infile;
    std::string outfile;

};


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


}
