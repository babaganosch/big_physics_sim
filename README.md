# Big Physics Simulation

## Features
* Objects rendered in one single draw call as sprites on quads via vertex array
* Objects collide with each other
* Some kind of basic (shitty) space partitioning
* ???
* Profit

## Stats
On my PC I can simulate about ~7000 objects in a 1400x900 world. I would like to get to about 30-50k.

__Specs:__
* Ubuntu 20.04.1
* 11th Gen Intel(R) Core(TM) i7-11700KF @ 3.60GHz
* Nvidia Geforce RTX 3070

My space partitioning is very naive and just divides room in cells twice the size of objects max-radius. This leaves a good amount of empty cells in half-empty rooms after objects settle at the bottom. I believe there's quite some room for optimizations in the code all together but I should probably try to implement some kind of multithreading or something on the collision checks. Would be cool to utilize compute shaders, CUDA or something..

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