Distributed ray tracer was implemented with motion blur, depth of field and anti-aliasing based on the code given in the reference.tar.gz. and Assignment 1. A large part of the modified code specially the parser and the input files was inspired from Computer Graphics II (Spring 2009) given in Utah University
<http://www.cs.utah.edu/~acumming/cs6620/Computer_Graphics_II/Computer_Graphics_II_Spring_2009/Computer_Graphics_II_Spring_2009.html>

The modifications in this assignment are listed below:

- Adding thinlens camera for depth of field with sampling frequency specified from "lenssamplingfrequency" in the .scn file 
- Computing the shaing color by shooting more than one ray per pixel (specified through "pixelsamplingfrequency" in .scn file)
- Adding movement to the primitives (sphere, triangles and polygons) to get motion blur effects and sampling over time (specified from "timesamplingfrequency" in the .scn file)

The code was developed under Windows environment (Visual Studio 2013). It was tested to run under Linux environment using Cygwin. Using Cygwin, I was able to cd to where the code is, then 'cmake CMakeLists.txt' followed by 'make' should produce the executable (specter.exe). 

The input files can be find under "scenes" sub-directory. multiple images are included where 
 - 'basic' in the file name means no effects are included
 - 'depth' (followed by lens radius and focus distance) in the file name means depth of field is implemented
 - 'motion' (followed by speed and sampling frequency over the lens) in the file name means that motion blur effect is implemented
 - 'anti' (followed by the sampling frequency per pixel) in the file name means that anti-aliasing is implemented 
 
The "Scene.scn" was used to experimented with different parameters and effects. "Cornell.scn" was used to test the depth of field over polygons and triangles. "room_with_spheres.scn" was used to test motion blur on polygons and simulated a falling roof. Additional images are included also. 

-ahmed
