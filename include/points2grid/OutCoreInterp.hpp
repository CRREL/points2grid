/*
*
COPYRIGHT AND LICENSE

Copyright (c) 2011 The Regents of the University of California.
All rights reserved.

Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following
conditions are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following
disclaimer in the documentation and/or other materials provided
with the distribution.

3. All advertising materials mentioning features or use of this
software must display the following acknowledgement: This product
includes software developed by the San Diego Supercomputer Center.

4. Neither the names of the Centers nor the names of the contributors
may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*
*
* Based on the notes by Prof. Ramon Arrowsmith(ramon.arrowsmith@asu.edu)
* Authors: Han S Kim (hskim@cs.ucsd.edu), Sriram Krishnan (sriram@sdsc.edu)
*
*/

#pragma once

#include <string.h>
#include <string>
#include <iostream>
#include <queue>
#include <list>

#include <points2grid/CoreInterp.hpp>

#include <points2grid/GridPoint.hpp>
#include <points2grid/GridMap.hpp>
#include <points2grid/export.hpp>

using namespace std;

class UpdateInfo;

class P2G_DLL OutCoreInterp : public CoreInterp
{
public:
    OutCoreInterp() {};
    OutCoreInterp(double dist_x, double dist_y,
                  int size_x, int size_y,
                  double r_sqr,
                  double _min_x, double _max_x,
                  double _min_y, double _max_y,
                  int _window_size);
    ~OutCoreInterp();

    virtual int init();
    virtual int update(double data_x, double data_y, double data_z);
    virtual int finish(const std::string& outputName, int outputFormat, unsigned int outputType);
    virtual int finish(const std::string& outputName, int outputFormat, unsigned int outputType, double *adfGeoTransform, const char* wkt);

private:
    void updateInterpArray(int fileNum, double data_x, double data_y, double data_z);
    void update_first_quadrant(int fileNum, double data_z, int base_x, int base_y, double x, double y);
    void update_second_quadrant(int fileNum, double data_z, int base_x, int base_y, double x, double y);
    void update_third_quadrant(int fileNum, double data_z, int base_x, int base_y, double x, double y);
    void update_fourth_quadrant(int fileNum, double data_z, int base_x, int base_y, double x, double y);

    void updateGridPoint(int fileNum, int x, int y, double data_z, double distance);
    int findFileNum(double data_y);
    void finalize();
    int outputFile(const std::string& outputName, int outputFormat, unsigned int outputType, double *adfGeoTransform, const char* wkt);
    void get_temp_file_name(char *fname, size_t fname_len);

public:
    static const unsigned int QUEUE_LIMIT = 1000;

private:
    double radius_sqr;

    int overlapSize;
    int local_grid_size_x;
    int local_grid_size_y;

    int numFiles;
    list<UpdateInfo> *qlist;
    GridMap **gridMap;
    int openFile;
};

class UpdateInfo
{
public:
    UpdateInfo() : data_x(0), data_y(0), data_z(0) {};
    UpdateInfo(double x, double y, double z) : data_x(x), data_y(y), data_z(z) {};

public:
    double data_x;
    double data_y;
    double data_z;
};

