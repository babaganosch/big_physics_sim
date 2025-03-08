# Big Physics Simulation

## Features
* Objects rendered in one single draw call as quads via VA with positions and colors
* Objects collide with each other (naive method)

TODO:
* Space partitioning used for object collision
* ???
* Profit

## PreReq
SFML - Easy window handling, rendering, events etc.
```
sudo apt-get install libsfml-dev
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Project

```bash
./big_physics_sim
```