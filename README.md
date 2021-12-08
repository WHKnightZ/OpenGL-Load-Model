# OpenGL-Load-Model

OpenGL Load Wavefront 3D Model 

## How to use:
- Choose model to display: `std::string model_name = "Models/Model_Name.obj";`
- Drag Right mouse to rotate
- Scroll mouse to zoom in/out

## How to build:
- Set up OpenGL, GLUT 
```
sudo apt-get update
sudo apt-get install mesa-common-dev libglu1-mesa-dev freeglut3-dev
```
For Arch Linux:
By default mesa should be installed, and libglu1 is included in [mesa package](https://archlinux.org/packages/extra/x86_64/mesa/). So you don't need to manually install it  
```
sudo pacman -Syu && sudo pacman -S mesa freeglut // Without yay installed
yay && yay -S mesa freeglut //With yay installed
```
- Clone Library
`git clone https://github.com/WHKnightZ/Library`
- Clone this project and go to source
`git clone https://github.com/WHKnightZ/OpenGL-Load-Model.git && cd OpenGL-Load-Model`
- Build with make
`make`
- Or build by g++
`g++ main.cpp -o load_model -lGL -lGLU -lglut`
- And run
`./load_model`

## Image:

![Image](https://github.com/WHKnightZ/OpenGL-Load-Model/blob/master/test.png)

## References:

- https://en.wikipedia.org/wiki/Wavefront_.obj_file
- https://www.loc.gov/preservation/digital/formats/fdd/fdd000507.shtml
- http://paulbourke.net/dataformats/mtl/
- https://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
