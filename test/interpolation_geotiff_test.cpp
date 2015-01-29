#include <gtest/gtest.h>
#include <points2grid/Interpolation.hpp>

#include "gdal_priv.h"

#include <points2grid/Global.hpp>

#include "fixtures.hpp"



namespace points2grid
{


namespace
{


class InterpolationGeotiffTest : public FourPointsTest
{};


}


TEST_F(InterpolationGeotiffTest, GeotiffHeaders)
{
    Interpolation interp(1, 1, 1, 3, INTERP_INCORE);
    interp.init(infile, INPUT_ASCII);
    interp.interpolation(infile, outfile, INPUT_ASCII, OUTPUT_FORMAT_ALL, OUTPUT_TYPE_ALL);

    GDALDataset *dataset;
    GDALAllRegister();
    dataset = (GDALDataset *) GDALOpen((outfile + ".idw.tif").c_str(), GA_ReadOnly);
    ASSERT_TRUE(dataset != NULL);
    double adfGeoTransform[6];
    EXPECT_EQ(dataset->GetGeoTransform(adfGeoTransform), CE_None);
    EXPECT_DOUBLE_EQ(adfGeoTransform[0], 0.5);
    EXPECT_DOUBLE_EQ(adfGeoTransform[1], 1.0);
    EXPECT_DOUBLE_EQ(adfGeoTransform[2], 0.0);
    EXPECT_DOUBLE_EQ(adfGeoTransform[3], 2.5);
    EXPECT_DOUBLE_EQ(adfGeoTransform[4], 0.0);
    EXPECT_DOUBLE_EQ(adfGeoTransform[5], -1.0);
    delete dataset;
}


}
