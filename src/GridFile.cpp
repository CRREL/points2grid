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
#include <points2grid/GridFile.hpp>
#include <float.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

GridFile::GridFile(int id, char *fname, int size_x, int size_y)
: m_id(id)
, m_size_x(size_x)
, m_size_y(size_y)
, m_inMemory(false)
, m_firstMap(true)
, m_filename(fname)
{

}

GridFile::~GridFile()
{
    unmap();
#ifdef _WIN32
    _unlink(m_filename.c_str());
#else
    unlink(m_filename.c_str());
#endif
}

int GridFile::getId()
{
    return m_id;
}

// memory map
int GridFile::map()
{
    // mapping is a no-op if the file is already mapped.
    if (m_inMemory) {
        return 0;
    }
    
    boost::iostreams::mapped_file_params params;
    params.path = m_filename;
    
    if (m_firstMap) {
        params.new_file_size = sizeof(GridPoint) * m_size_x * m_size_y;
    }
    
#ifndef OLD_BOOST_IOSTREAMS
    params.flags = boost::iostreams::mapped_file::readwrite;
#else
    params.mode = std::ios_base::in | std::ios_base::out;
#endif

    try {
        m_mf.open(params);
        interp = (GridPoint *) m_mf.data();
    }
    catch(std::exception& e) {
        cerr << e.what() << endl;
        return -1;
    }

    if (m_firstMap) {
        // initialize every point in the file
        GridPoint init_value = {DBL_MAX, -DBL_MAX, 0, 0, 0, 0, 0, 0};
        for(int i = 0; i < m_size_x * m_size_y; i++)
            memcpy(interp + i, &init_value, sizeof(GridPoint));
        cout << m_id << ". file size: " << params.new_file_size << endl;
        m_firstMap = false;
    }
    
    m_inMemory = true;
    return 0;
}

int GridFile::unmap()
{
    if(m_inMemory)
    {
        m_mf.close();
        m_inMemory = false;
        interp = NULL;
    }

    return 0;
}

bool GridFile::isInMemory()
{
    return m_inMemory;
}

unsigned int GridFile::getMemSize()
{
    return m_size_x * m_size_y * sizeof(GridPoint);
}
