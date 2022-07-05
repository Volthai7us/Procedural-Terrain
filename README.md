# Procedural Terrain Generation
<img src="https://serving.photos.photobox.com/59100808114fbc5dd930796498918307cc034d516cd7c18b8ac985411c87e4aa0107f553.jpg" width="500" height="500" />
<img src="https://serving.photos.photobox.com/855100253e8638712e116040a5b62f5a913fe7ef9b2e26d8c7cbd15f7d8602f575d93fb3.jpg" width="500" height="500" />

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
