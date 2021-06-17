Author: Martin Fleisz
E-mail: martin.fleisz@kabsi.at

Game Programming Gems 6 - Adaptive Binary Tree sample code

This Visual C++ project loads the geometry data of one or more .ply files, 
creates an Adaptive Binary Tree and renders the model (using frustum culling
to remove unseen parts). For a detailed description of the ABT, please refer 
to the article in the book.

Notes:

- The application searches for input data in the /data directory and scans this and all sub-directories 
  for .ply files for loading

- The application uses the RPLY library. Here you can find more information about it:   http://www.cs.princeton.edu/~diego/professional/rply/

- A good source for ply models is the page of the UNC Walkthru Project that provides a model of a huge
  power plant split in several sub-models. I never tried to load the whole model and I assume it would take
  quite some time (and memory) to build the ABT - so be careful how much data you load from the model.
  http://www.cs.unc.edu/~geom/Powerplant/