
***************************************************
*   HEXCell - a framework for hexagonal rasters   *
***************************************************

>> OBJECTIVE

Hexagonal latices have some benefits over square lattices but they aren't widely used.
This framework aims to facilitate the potentialy cumbersome implementation of hexagonal latices in an application. It is highly generica and flexible and adresses a couple of issues a developer might be facing when dealing with hexagonal latices.


>> COVERED FEATURES

- Data Storage

Descendants of the generic DataGrid<> class deal with the storage of data organized in a hexagonal lattice.

- Adressing

Descendants of the generic AddressingScheme<> class associate the data with adresses. These adressing schemes are used to ccess the data. On top of that they provide utility functionality used by other components of the framework.

- Neighbourhoods

A number of generic CellIterator<> and CellEnumator<> classes allow data to be read and writen in an intuitive way. Iteration over the latice in arbitrary directions is supported as well as an enumeration of all cells in a specific neighbourhood or rectangular or radial regions of the latice.

- Visualisation

The two included generic classes CustomGridRenderer<> and SimpleGridRenderer<> provide a highly customizable way to visualize hexagonaly organized data.

- Conversion

When using hexagonal lattices in a context where the data has to be converted from and into a rectangular lattice supplying a suitable algorithm to perform the conversion can prove to be quite a challenge. The framework aids the developer by providing a generic approach for nearest-neighbour and linear interpolation as well as a geometric accurate approach that's based on the actual cell shapes.


>> EXAMPLE APPLICATIONS

a) Converter Lab

This example shows the converter functionality of the Framework.
Open a bitmap type file to see it displayed in the left window. Now a click on "Convert to Hex" will create an image on hexagonal lattice. The occupied cells is given by "Grid Width" and "Grid Height". The "Sampling" combobox allows to set different kinds of conversion algorithm.
The hexagonal immage is visualized on the right side. "Cell Size" sets the displaying size of a hexagonal pixel. "Convert to Square" will convert the hexagonal immage back into a normal bitmap.

b) Hex Life 

This example shows how a simple cellular automata on a hexagonal lattice can be implemented using the framework. It also shows how easy it is to customize the visualization using delegates. The settings allow the user to chose between 3 different visualizations, the first one shows the current state of the cell which can be either on (green cell) or off (not drawn). The remaining tow settings visualize not the current state but the recent activity of a cell i.e. how often the cells state changed in the last couple of iterations. While the first of the two visualizations uses a the SimpleGrindRenderer the second is based on the CustomGridRenderer where CellPaint events trigger Eventhandlers provided by the framework's client.

c) EdgeDetection

This example shows how the framework can be used to define filters for the use with hexagonal image data. There are three Laplace operators implemented that allow to perform edge detection on an image.
The first two operate on normal bitmap data, where one consideres only four neighbouring pixels (Von-Neuman-Neighbourhood) and the other consideres diagonal neighbours too and thus a total of eight neighbouring pixels (Moore-Neighourhood).
On hexagonal lattices there is no such ambiguity - there are six äquidistant neihbours which are considered by the third Laplace operator. 

Applying the hexagonal operator on a Bitmap requires a conversion of the immage data onto a hexagonal lattaice and back on a square lattice afterwards thus explaining the substantial higher processing time. 