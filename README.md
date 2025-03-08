# Big Physics Simulation

## Ideas
* Render objects as sprites
* Obviously utilize some kind of space partitioning for updating objects (Although, start with some naive method to get things rolling..)
* Assignment hints about SIMD, Multithreading, Shaders. Perhaps start out with simply multithreading and test out CUDA if there's time..

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