#pragma once

#include <gtest/gtest.h>

#include "config.hpp"


namespace points2grid
{


class FourPointsTest : public ::testing::Test
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
        std::remove((outfile + ".den.grid").c_str());
        std::remove((outfile + ".idw.grid").c_str());
        std::remove((outfile + ".max.grid").c_str());
        std::remove((outfile + ".mean.grid").c_str());
        std::remove((outfile + ".min.grid").c_str());
        std::remove((outfile + ".std.grid").c_str());
        std::remove((outfile + ".den.tif").c_str());
        std::remove((outfile + ".idw.tif").c_str());
        std::remove((outfile + ".max.tif").c_str());
        std::remove((outfile + ".mean.tif").c_str());
        std::remove((outfile + ".min.tif").c_str());
        std::remove((outfile + ".std.tif").c_str());
    }

    std::string infile;
    std::string outfile;

};

class ExcludePointsTest : public ::testing::Test
{
public:

    virtual void SetUp()
    {
        infile = get_test_data_filename("example.las");
        outfile = get_test_data_filename("outfile");
    }

    virtual void TearDown()
    {
        std::remove((outfile + ".den.asc").c_str());
        std::remove((outfile + ".den.grid").c_str());
        std::remove((outfile + ".den.tif").c_str());
    }

    std::string infile;
    std::string outfile;

};


}
