# raytracer

Implements a raytracer based on the book [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). 

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake -G "Visual Studio 16 2019" ..
raytracer/build $ start CS312-Raytracer.sln
```

Your solution file should contain multiple projects, such as `gradient` and `normals`.
To run from the git bash command shell, 

```
raytracer/build $ ../bin/Debug/gradient
raytracer/build $ ../bin/Debug/normals
```

*macOS*

Open terminal to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake ..
raytracer/build $ make
```

To run each program from build, you would type

```
raytracer/build $ ../bin/gradient
raytracer/build $ ../bin/normals
```

##Required materials implemented
![materials](https://github.com/shaili-regmi/raytracer/blob/main/materials.png)


##Unique feature implemented: 
Supports changing the camera position using look at and up vectors.
![camera-change](https://github.com/shaili-regmi/raytracer/blob/main/camera-changed-materials.png)


##Primitives implemented:

Triangles:
![triangle](https://github.com/shaili-regmi/raytracer/blob/main/triangle.png)


Planes:
![plane](https://github.com/shaili-regmi/raytracer/blob/main/plane.png)


##Unique images created:

*Note that the code used for creating the images are commented out at the end of the materials.cpp file*

1. Phong sphere helices - two helices of phong spheres with a central red metallic sphere viewed from two different viewpoints. The two helices have slightly different radii and pitch to accomodate a better picture at the two viewpoints.


Top-down view:
![sphere helix viewpoint 1](https://github.com/shaili-regmi/raytracer/blob/main/unique-image1.png)


Side view (XY quadrant):
![sphere helix viewpoint 2](https://github.com/shaili-regmi/raytracer/blob/main/unique-image1-camera-changed.png)


2. Platonic solid using triangles and a plane primitive base - viewed from the XY quadrant with the z-direction being upwards:
![platonic solid and plane](https://github.com/shaili-regmi/raytracer/blob/main/unique-image2.png)