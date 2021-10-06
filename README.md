# Convex Hull calculation application
## Brief information

The application takes the coordinates of points or planes as input (given by inequalities) and calculates with which vertices (planes) it is possible to create a convex polyhedron of maximum volume.  
Output: a graphical representation of the polyhedron and a textual representation of the vertices (coordinates)  

Example of work:  
![Screenshot of the application](../assets/assets/demo-work.gif?raw=true)

## Launching the app
#### Installation
OpenGL and freeglut are used.  
It is advisable to work in Visual Studio with vcpkg installed.  
Installation instructions can be found here: https://vcpkg.io/en/getting-started.html  
Next, install OpenGL, freeglut  
`vcpkg install freeglut:x64-windows`  
`vcpkg install integrate`  

Everything will be implemented automatically in visual studio and no additional settings need to be made.  

#### Launch
In the command line, run the command:  
`Convex_poly.exe input.txt`  
Or without an argument input.txt, then the points will need to be entered manually.  

where *input* is the name of the data file
input.txt:  
```
V
9
0 0 2
2 0 2
0 2 2
1 0 0
2 0 0
0 1 0
0 2 0
2 2 0
```
1st row - data type, V - vertices, H - Inequalities (planes)  
2nd line - number of points  
The remaining rows are the rows of vertex coordinates (x, y, z) - integer if the data type is V.  
If the data type is H, then inequalities are introduced, similarly:  
input.txt:
```
H
7
0 0 1 2
0 -1 0 0
-1 0 0 0
-2 -2 -1 -2
1 0 0 2
0 1 0 2
0 0 -1 0
```
The data can be interpreted as a system of inequalities:  
![System of inequalities](../assets/assets/inequality1.png?raw=true)  

## Application Results
The program will display the following data:  
* All vertices of the desired convex polyhedron (coordinates and their naming)
* Adjacency matrix, which tells how the vertices are connected  
1 - Vertices are connected by a straight line, 0 - not  
* The face defined by the inequality
* And straight lines belonging to this face
* Graphical representation of the convex hull in the form of a 3D graph along the XYZ axes

### Note:
1) To display the graphics, you need to open a full-screen window with a polyhedron.
2) You can run a graph or move away, LCM - zoom in, PCM - move away
3) The rotation of the graph is set at the very beginning of the application