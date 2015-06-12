#include <gtest/gtest.h>
#include <points2grid/Interpolation.hpp>

#include <points2grid/config.h>
#include <points2grid/Global.hpp>
#include <cmath>

#include "fixtures.hpp"


namespace points2grid {


namespace {

    class LASFilterTest : public ExcludePointsTest {
    };

}

TEST_F(LASFilterTest, InitLAS) {
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    int retval = interp.init(infile, INPUT_LAS);
    EXPECT_EQ(0, retval);
}


TEST_F(LASFilterTest, InterpolateExcludeClass) {
    std::vector<int> exclude_class;
    exclude_class.push_back(1);

    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_LAS);
    interp.setLasExcludeClassification(exclude_class);

    int retval = interp.interpolation(infile, outfile, INPUT_LAS, OUTPUT_FORMAT_ARC_ASCII, OUTPUT_TYPE_ALL);
    EXPECT_EQ(0, retval);

    EXPECT_EQ(276, interp.las_point_count);
}

TEST_F(LASFilterTest, InterpolateFirstReturns) {
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_LAS);
    interp.setLasExcludeReturn(true);

    int retval = interp.interpolation(infile, outfile, INPUT_LAS, OUTPUT_FORMAT_ARC_ASCII, OUTPUT_TYPE_ALL);
    EXPECT_EQ(0, retval);

    EXPECT_EQ(925, interp.las_point_count);
}

TEST_F(LASFilterTest, InterpolateLastReturns) {
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_LAS);
    interp.setLasExcludeReturn(false);

    int retval = interp.interpolation(infile, outfile, INPUT_LAS, OUTPUT_FORMAT_ARC_ASCII, OUTPUT_TYPE_ALL);
    EXPECT_EQ(0, retval);

    EXPECT_EQ(901, interp.las_point_count);
}


}