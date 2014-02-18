Points2Grid
------------

Points2Grid generates Digital Elevation Models (DEM) using a local gridding
method. The local gridding algorithm computes grid cell elevation using a
circular neighbourhood defined around each grid cell based on a radius
provided by the user. This neighbourhood is referred to as a bin, while the
grid cell is referred to as a DEM node. Up to four values — minimum,
maximum, mean, or inverse distance weighted (IDW) mean — are computed for
points that fall within the bin. These values are then assigned to the
corresponding DEM node and used to represent the elevation variation over
the neighbourhood represented by the bin. If no points are found within a
given bin, the DEM node receives a value of null. The Points2Grid service
also provides a null filing option, which applies an inverse distance
weighted focal mean via a square moving window of 3, 5, or 7 pixels to fill
cells in the DEM that have null values.

More information about Points2Grid (SVN, downloads, mailing list) can be found
at: http://www.opentopography.org/index.php/resources/otforge/points2grid.

HISTORY
-------

The points2grid source tree at CRREL's github repository is a fork 
of the work that was done at ASU and OpenTopography. It is probably the 
most up-to-date version of the code at this time. Note that CRREL has 
not functionally changed the algorithms, however, and most of the 
tweaks have been in how points2grid gets its data. This CRREL edition 
contains its own LAS-reading code to eliminate all significant 
external dependencies. This points2grid tree can also be linked to PDAL 
as a PDAL writer to output grids using PDAL data sources.

INSTALLATION
------------
Please read the INSTALL file for details on the installation process.

USE
---
If any of your pre-requisites (curl) are installed on non-standard
location, please set your LD_LIBRARY_PATH accordingly. 

To view complete usage information, please type "./points2grid" from the
command-line.