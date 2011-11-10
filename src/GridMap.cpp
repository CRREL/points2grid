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


#include <points2grid/GridMap.hpp>


GridMap::GridMap(int id, 
                 int size_x, 
                 int lower_bound, 
                 int upper_bound, 
                 int overlap_lower_bound, 
                 int overlap_upper_bound, 
                 bool initialized, 
                 char *fname)
{
    m_id = id;
    m_lowerBound = lower_bound;
    m_upperBound = upper_bound;
    m_overlapLowerBound = overlap_lower_bound;
    m_overlapUpperBound = overlap_upper_bound;

    m_gridFile = new GridFile(m_id, fname, size_x, m_overlapUpperBound - m_overlapLowerBound + 1);
    
    if (m_gridFile != 0)
        m_initialized = initialized;

}
GridMap::~GridMap()
{
    if(m_gridFile != 0)
        delete m_gridFile;
}

int GridMap::getLowerBound()
{
    return m_lowerBound;
}
int GridMap::getUpperBound()
{
    return m_upperBound;
}
int GridMap::getOverlapLowerBound()
{
    return m_overlapLowerBound;
}
int GridMap::getOverlapUpperBound()
{
    return m_overlapUpperBound;
}

bool GridMap::isInitialized()
{
    return m_initialized;
}
int GridMap::getId()
{
    return m_id;
}
GridFile *GridMap::getGridFile()
{
    return m_gridFile;
}

void GridMap::setLowerBound(int lower_bound)
{
    m_lowerBound = lower_bound;
}
void GridMap::setUpperBound(int upper_bound)
{
    m_upperBound = upper_bound;
}
void GridMap::setOverlapLowerBound(int overlap_lower_bound)
{
    m_overlapLowerBound = overlap_lower_bound;
}
void GridMap::setOverlapUpperBound(int overlap_upper_bound)
{
    m_overlapUpperBound = overlap_upper_bound;
}
void GridMap::setInitialized(bool initialized)
{
    m_initialized = initialized;
}
void GridMap::setId(int id)
{
    m_id = id;
}
/*
void GridMap::setGridFile(string fname)
{
    if(gridFile != NULL)
	delete gridFile;

    gridFile = new GridFile(id, fname, _size_x, _overlap_upper_bound - _overlap_lower_bound + 1);
}
*/

