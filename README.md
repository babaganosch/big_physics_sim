# Big Physics Simulation

## Features
* Objects rendered in one single draw call as sprites on quads via vertex array
* Objects collide with each other
* Some kind of basic (shitty) space partitioning
* Multithreading on collision checks (although needs refinement, pretty much copy-paste from stack overflow on threading right now..)
* ???
* Profit

## Stats
On my PC I can simulate about ~12000 objects (2-6px radii) in a 1800x1200 world. I would like to get to about 30-50k.

__Specs:__
* Ubuntu 20.04.1
* 11th Gen Intel(R) Core(TM) i7-11700KF @ 3.60GHz
* Nvidia Geforce RTX 3070

My space partitioning is very naive and just divides room in cells twice the size of objects max-radius. This leaves a good amount of empty cells in half-empty rooms after objects settle at the bottom. I believe there's quite some room for optimizations.

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