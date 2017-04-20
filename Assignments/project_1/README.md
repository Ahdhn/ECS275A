Basic recursive ray tracer was implemented based on the code given in the reference.tar.gz. A large part of the modified code specially the parser and the input files was inspired from Computer Graphics II (Spring 2009) given in Utah University
<http://www.cs.utah.edu/~acumming/cs6620/Computer_Graphics_II/Computer_Graphics_II_Spring_2009/Computer_Graphics_II_Spring_2009.html>

The modifications are listed below:

- Parser for triangles, n-polygons (only squares are tested so far), Phong material (to enable specular effects) 
- To support triangles and polygons intersection, two new classes were added for them
- To support shading specular effects, new class added for Phong material 

The code was developed under Windows environment (Visual Studio 2013). It was tested to run under Linux environment using Cygwin. Using Cygwin, I was able to cd to where the code is, then 'cmake CMakeLists.txt' followed by 'make' should produce the executable (specter.exe). 

The input files can be find under "scenes" sub-directory. For all given input files, the code runs for less a minute before the image is produced. For spiral.scn, it could take a little more than a minute. For teapot.scn, it take few minutes. 

-ahmed
