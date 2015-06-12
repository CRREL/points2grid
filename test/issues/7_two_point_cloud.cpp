/*
*
COPYRIGHT AND LICENSE

Copyright (c) 2015 Peter J. Gadomski <pete.gadomski@gmail.com>
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
*/

#include <gtest/gtest.h>

#include <points2grid/InCoreInterp.hpp>


namespace points2grid
{


TEST(Issue7, TwoPointCloud)
{
    InCoreInterp interp(1e4, 1e4, 1, 1, 1e4, 0, 10, 0, 10, 3);
    interp.init();
    interp.update(0, 0, 10);
    interp.update(10, 10, 0);
    interp.calculate_grid_values();
    GridPoint point = interp.get_grid_point(0, 0);
    EXPECT_EQ(2U, point.count);
    EXPECT_EQ(5, point.Zmean);
    EXPECT_EQ(0, point.Zmin);
    EXPECT_EQ(10, point.Zmax);
}


}
