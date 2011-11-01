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

#include "config.h"
#include "GridFile.h"
#include <float.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

GridFile::GridFile(int id, char *_fname, int _size_x, int _size_y)
{
    boost::iostreams::mapped_file mf;
    ID = id;
    strncpy(fname, _fname, sizeof(fname));
    size_x = _size_x;
    size_y = _size_y;
    inMemory = false;
 }

GridFile::~GridFile()
{
    if(inMemory == true){
        mf.close();
        inMemory = false;
        interp = NULL;
    }

    unlink(fname);

    //cout << "file closed: " << ID << endl;
}

int GridFile::getId()
{
    return ID;
}

// memory map
int GridFile::map()
{
    boost::iostreams::mapped_file_params params;
    params.path = fname;
    params.new_file_size = sizeof(GridPoint) * size_x * size_y;

#ifndef OLD_BOOST_IOSTREAMS
    params.flags = boost::iostreams::mapped_file::readwrite;
#else
    params.mode = std::ios_base::in | std::ios_base::out;
#endif

    try {
        mf.open(params);
        interp = (GridPoint *) mf.data();
    }
    catch(std::exception& e) {
        fprintf(stderr, "mmap error %d(%s) \n", errno, strerror(errno));
        return -1;
    }
    inMemory = true;

    // initialize every point in the file
    GridPoint init_value = {DBL_MAX, -DBL_MAX, 0, 0, 0, 0, 0, 0};
    for(int i = 0; i < size_x * size_y; i++)
        memcpy(interp + sizeof(GridPoint)*i, &init_value, sizeof(GridPoint));
    cout << ID << ". file size: " << params.new_file_size << endl;

    return 0;
}

int GridFile::unmap()
{
    // have to delete previous information??
    // have to overwrite

    // we can track the changes but that scheme requires memory usage.
    // But our main goal is to fully utilize memory. 
    
    if(interp != NULL)
    {
        mf.close();
	    inMemory = false;
	    interp = NULL;
    }

    return 0;
}

bool GridFile::isInMemory()
{
    return inMemory;
}

unsigned int GridFile::getMemSize()
{
    return size_x * size_y * sizeof(GridPoint);
}
