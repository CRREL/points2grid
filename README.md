# points2grid

**points2grid** generates Digital Elevation Models (DEM) using a local gridding method.
The local gridding algorithm computes grid cell elevation using a circular neighbourhood defined around each grid cell based on a radius provided by the user.
This neighbourhood is referred to as a bin, while the grid cell is referred to as a DEM node.
Several values, including minimum, maximum, mean, and inverse distance weighted (IDW) mean, are computed for points that fall within the bin.
These values are then assigned to the corresponding DEM node and used to represent the elevation variation over the neighbourhood represented by the bin.
If no points are found within a given bin, the DEM node receives a value of null.
**points2grid** also provides a null filling option, which applies an inverse distance weighted focal mean via a square moving window of 3, 5, or 7 pixels to fill cells in the DEM that have null values.

More information about **points2grid** can be found on its [OpenTopography page](http://opentopo.sdsc.edu/gridsphere/gridsphere;jsessionid=3E6AD86428FB61C15BB10A9972E2EE8B?gs_action=viewTool&cid=contributeframeportlet&toolId=201) or [SourceForge page](http://sourceforge.net/projects/otforge/files/points2grid/1.0.1/).


## History

The **points2grid** source tree at [CRREL's github repository](https://github.com/CRREL/points2grid) is a fork of the work that was done at ASU and OpenTopography.
It is probably the most up-to-date version of the code at this time.
Note that CRREL has not functionally changed the algorithms, however, and most of the tweaks have been in how **points2grid** gets its data.
This CRREL edition contains its own LAS-reading code to eliminate all significant external dependencies.
This **points2grid** tree can also be linked to PDAL as a PDAL writer to output grids using PDAL data sources.


## Installation

See INSTALL.md in this source tree for installation instructions.


## Usage

If any of your pre-requisites (curl) are installed on non-standard location, please set your `LD_LIBRARY_PATH` accordingly. 
To view complete usage information, please type `./points2grid` from the command-line.
