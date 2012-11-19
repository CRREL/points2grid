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

#include <points2grid/config.h>
#include <points2grid/Interpolation.hpp>
#include <points2grid/Global.hpp>

#include <string.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#ifdef PDAL_FOUND
#include <pdal/pdal.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/PointBuffer.hpp>
#include <pdal/StageIterator.hpp>
#include <pdal/Dimension.hpp>
#include <pdal/Schema.hpp>
#else /* LIBLAS_FOUND */

#ifdef LIBLAS_FOUND
extern "C" {
    #include <liblas/capi/liblas.h> // the C API is stable across liblas versions
}
#endif
#endif

#include <boost/scoped_ptr.hpp>


#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <string.h>

/////////////////////////////////////////////////////////////
// Public Methods
/////////////////////////////////////////////////////////////

Interpolation::Interpolation(double x_dist, double y_dist, double radius,
                             int _window_size, int _interpolation_mode = INTERP_AUTO) : GRID_DIST_X (x_dist), GRID_DIST_Y(y_dist)
{
    data_count = 0;
    radius_sqr = radius * radius;
    window_size = _window_size;
    interpolation_mode = _interpolation_mode;

    min_x = DBL_MAX;
    min_y = DBL_MAX;

    max_x = -DBL_MAX;
    max_y = -DBL_MAX;
}

Interpolation::~Interpolation()
{
    delete interp;
}

int Interpolation::init(char *inputName, int inputFormat)
{
    //unsigned int i;

    //struct tms tbuf;
    clock_t t0, t1;


    //////////////////////////////////////////////////////////////////////
    // MIN/MAX SEARCHING
    // TODO: the size of data, min, max of each coordinate
    //       are required to implement streaming processing....
    //
    // This code can be eliminated if database can provide these values
    //////////////////////////////////////////////////////////////////////

    //t0 = times(&tbuf);
    t0 = clock();

    if(inputName == NULL)
    {
        cout << "Wrong Input File Name" << endl;
        return -1;
    }

    printf("inputName: '%s'\n", inputName);

    if (inputFormat == INPUT_ASCII) {
        FILE *fp;
        char line[1024];
        double data_x, data_y;
        //double data_z;

        if((fp = fopen(inputName, "r")) == NULL)
        {
            cout << "file open error" << endl;
            return -1;
        }

        // throw the first line away - it contains the header
        fgets(line, sizeof(line), fp);

        // read the data points to find min and max values
        while(fgets(line, sizeof(line), fp) != NULL)
        {
            data_x = atof(strtok(line, ",\n"));
            if(min_x > data_x) min_x = data_x;
            if(max_x < data_x) max_x = data_x;

            data_y = atof(strtok(NULL, ",\n"));
            if(min_y > data_y) min_y = data_y;
            if(max_y < data_y) max_y = data_y;

            data_count++;


            /*
            // tokenizing
            string strLine(line);

            // first token
            string::size_type pos = strLine.find_first_of(",",0);
            string::size_type lastPos = strLine.find_first_not_of(",",0);

            string strX = strLine.substr(lastPos, pos - lastPos);

            // second token
            lastPos = strLine.find_first_not_of(",", pos);
            pos = strLine.find_first_of(",", lastPos);

            string strY = strLine.substr(lastPos, pos - lastPos);

            // third token
            lastPos = strLine.find_first_not_of(",", pos);
            pos = strLine.find_first_of("\n", lastPos);

            string strZ = strLine.substr(lastPos, pos - lastPos);

            // data conversion
            arrX[data_count] = atof(strX.c_str());
            if(min_x > arrX[data_count]) min_x = arrX[data_count];
            if(max_x < arrX[data_count]) max_x = arrX[data_count];

            arrY[data_count] = atof(strY.c_str());
            if(min_y > arrY[data_count]) min_y = arrY[data_count];
            if(max_y < arrY[data_count]) max_y = arrY[data_count];

            arrZ[data_count++] = atof(strZ.c_str());
            */
        }

        fclose(fp);
    } else { // las input
#ifdef PDAL_FOUND
        try {
            
            pdal::Options options;
            options.add("filename", inputName);
            
            pdal::StageFactory factory;
            pdal::Stage* reader = factory.createReader("drivers.pipeline.reader", options);

            reader->initialize();
 
            pdal::Bounds<double> bounds = reader->getBounds();
            
            min_x = bounds.getMinimum(0);
            min_y = bounds.getMinimum(1);
            max_x = bounds.getMaximum(0);
            max_y = bounds.getMaximum(1);
            
            // boost::scoped_ptr<boost::StageSequentialIterator> iter(reader.createSequentialIterator());
            // iter->skip(m_pointNumber);
        } catch (std::runtime_error &e) {
            cout << "error while reading LAS file: verify that the input is valid LAS file" << e.what() << endl;
            return -1;
        }
#endif /* LIBLAS_FOUND */
#ifdef LIBLAS_FOUND
        LASReaderH lr = LASReader_Create(inputName);
        if (!lr) {
            LASError_Print("Could not open file to read.");
            return -1;
        }
        
        LASHeaderH lh = LASReader_GetHeader(lr);        
        if (!lh) {
            LASError_Print("error while reading LAS file: verify that the input is valid LAS file.");
            LASReader_Destroy(lr);    
            return -1;            
        }
        
        min_x = LASHeader_GetMinX(lh);
        min_y = LASHeader_GetMinY(lh);
        max_x = LASHeader_GetMaxX(lh);
        max_y = LASHeader_GetMaxY(lh);
        data_count = LASHeader_GetPointRecordsCount(lh);
        
        LASReader_Destroy(lr);
#endif  
    }

    t1 = clock();
    printf("Min/Max searching time: %10.2f\n", (double)(t1 - t0)/CLOCKS_PER_SEC);


    //t0 = times(&tbuf);

    //////////////////////////////////////////////////////////////////////
    // Intialization Step excluding min/max searching
    //////////////////////////////////////////////////////////////////////
    /*
      for(i = 0; i < data_count; i++)
      {
      arrX[i] -= min_x;
      arrY[i] -= min_y;
      //printf("%f,%f,%f\n", arrX[i] , arrY[i] ,arrZ[i]);
      }
    */

    cout << "min_x: " << min_x << ", max_x: " << max_x << ", min_y: " << min_y << ", max_y: " << max_y << endl;

    GRID_SIZE_X = (int)(ceil((max_x - min_x)/GRID_DIST_X)) + 1;
    GRID_SIZE_Y = (int)(ceil((max_y - min_y)/GRID_DIST_Y)) + 1;

    cout << "GRID_SIZE_X " << GRID_SIZE_X << endl;
    cout << "GRID_SIZE_Y " << GRID_SIZE_Y << endl;

    if (interpolation_mode == INTERP_AUTO) {
        // if the size is too big to fit in memory,
        // then construct out-of-core structure
        if(GRID_SIZE_X * GRID_SIZE_Y > MEM_LIMIT) {
            interpolation_mode= INTERP_OUTCORE;
        } else {
            interpolation_mode = INTERP_INCORE;
        }
    }

    if (interpolation_mode == INTERP_OUTCORE) {
        cout << "Using out of core interp code" << endl;;

        interp = new OutCoreInterp(GRID_DIST_X, GRID_DIST_Y, GRID_SIZE_X, GRID_SIZE_Y, radius_sqr, min_x, max_x, min_y, max_y, window_size);
        if(interp == NULL)
        {
            cout << "OutCoreInterp construction error" << endl;
            return -1;
        }

        cout << "Interpolation uses out-of-core algorithm" << endl;

    } else {
        cout << "Using incore interp code" << endl;

        interp = new InCoreInterp(GRID_DIST_X, GRID_DIST_Y, GRID_SIZE_X, GRID_SIZE_Y, radius_sqr, min_x, max_x, min_y, max_y, window_size);

        cout << "Interpolation uses in-core algorithm" << endl;
    }

    if(interp->init() < 0)
    {
        cout << "inter->init() error" << endl;
        return -1;
    }

    cout << "Interpolation::init() done successfully" << endl;

    return 0;
}

int Interpolation::interpolation(char *inputName,
                                 char *outputName,
                                 int inputFormat,
                                 int outputFormat,
                                 unsigned int outputType)
{
    int rc;
    //unsigned int i;
    double data_x, data_y;
    double data_z;

    //struct tms tbuf;
    //clock_t t0, t1;

    printf("Interpolation Starts\n");

    //t0 = times(&tbuf);

    //cout << "data_count: " << data_count << endl;

    /*
      if((rc = interp->init()) < 0)
      {
      cout << "inter->init() error" << endl;
      return -1;
      }
    */

    if (inputFormat == INPUT_ASCII) {
        FILE *fp;
        char line[1024];

        if((fp = fopen(inputName, "r")) == NULL)
        {
            printf("file open error\n");
            return -1;
        }

        // throw the first line away - it contains the header
        fgets(line, sizeof(line), fp);

        // read every point and generate DEM
        while(fgets(line, sizeof(line), fp) != NULL)
        {
            data_x = atof(strtok(line, ",\n"));
            data_y = atof(strtok(NULL, ",\n"));
            data_z = atof(strtok(NULL, ",\n"));

            data_x -= min_x;
            data_y -= min_y;

            //if((rc = interp->update(arrX[i], arrY[i], arrZ[i])) < 0)
            if((rc = interp->update(data_x, data_y, data_z)) < 0)
            {
                cout << "interp->update() error while processing " << endl;
                return -1;
            }
        }

        fclose(fp);
    } 

#ifdef LIBLAS_FOUND    
    else { // input format is LAS

        LASReaderH lr = LASReader_Create(inputName);
        if (!lr) {
            LASError_Print("Could not open file to read.");
            return -1;
        }
        
        LASPointH lp;
        while ((lp = LASReader_GetNextPoint(lr))) {
            data_x = LASPoint_GetX(lp);
            data_y = LASPoint_GetY(lp);
            data_z = LASPoint_GetZ(lp);
            
            data_x -= min_x;
            data_y -= min_y;
            
            if ((rc = interp->update(data_x, data_y, data_z)) < 0) {
                cout << "interp->update() error while processing " << endl;
                return -1;
            }
        }
        
        LASReader_Destroy(lr);

    }
#endif

    if((rc = interp->finish(outputName, outputFormat, outputType)) < 0)
    {
        cout << "interp->finish() error" << endl;
        return -1;
    }

    cout << "Interpolation::interpolation() done successfully" << endl;

    return 0;
}

void Interpolation::setRadius(double r)
{
    radius_sqr = r * r;
}

unsigned int Interpolation::getDataCount()
{
    return data_count;
}

unsigned int Interpolation::getGridSizeX()
{
    return GRID_SIZE_X;
}

unsigned int Interpolation::getGridSizeY()
{
    return GRID_SIZE_Y;
}

