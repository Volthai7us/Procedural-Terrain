# Procedural Terrain Generation

## Features

- Shading Mode can change via 'F' or 'f' button.
- There is 3 camera mode:
    * Focus on origin free camera
    * Above and behind of plane camera
    * Front of plane camera
- Plane can be turned by left arrow and right arrow.
- Plane can be accelerated by 'A' or 'a' and 'D' or 'd' 

## Languages

Written using C++

## Running

For compile in ubuntu

```

g++ *.cpp -lGL -lGLU -lglut -lGLEW -o Terrain

```

To run

```

./Terrain nor min max seed ba ts

nor = Number Of Recursion Iteration (Suggest: 8)
min = Start Minimum of Perturbation (Suggest: -.5)
max = Start Maximum of Perturbation (Suggest: .5)
seed = Terrain Seed. If -1 then seed is random. (Suggest: -1)
ba = If 1 Border Align is true. Else Border Align is false. (Suggest 1)
ts = Terrain Size (Suggest 5)
Example Run:
./Terrain 8 -.5 .5 -1 1 5

```