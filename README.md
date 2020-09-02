# OpenGL-Load-Model

OpenGL Load Wavefront 3D Model 

## How to build:
- Set up OpenGL, GLUT 
```
sudo apt-get update
sudo apt-get install mesa-common-dev libglu1-mesa-dev freeglut3-dev
```
- Clone this project and go to source
`git clone https://github.com/WHKnightZ/OpenGL-Load-Model.git && cd OpenGL-Load-Model`
- Build with make
`make`
- Or build by g++
`g++ main.cpp -o load_model -lGL -lGLU -lglut`
- And run
`./load_model`

## References:

- https://en.wikipedia.org/wiki/Wavefront_.obj_file
- https://www.loc.gov/preservation/digital/formats/fdd/fdd000507.shtml
- http://paulbourke.net/dataformats/mtl/
- https://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
