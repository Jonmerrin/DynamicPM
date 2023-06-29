# Surface Simplification Using Quadric Error Metrics

## USAGE
```
$ python3 build.py
$ ./lab <float tolerance> <string pathToObjFile>
```
where "tolerance" is how far apart two vertices can be for them to be candidates for contraction without an edge.


## CONTROLS
| control | Description |
| --- | ----------- |
| arrow keys | move camera (forward, back, left, right) |
| rshift | move camera upText |
| ctrl|move camera down |
|1 |decimate the mesh by 10% of the remaining triangles |
|2 |decimate the mesh by 20% of the remaining triangles |
|3 |decimate the mesh by 30% of the remaining triangles |
|4 |decimate the mesh by 40% of the remaining triangles |
|5 |decimate the mesh by 50% of the remaining triangles |
|6 |decimate the mesh by 60% of the remaining triangles |
|7 |decimate the mesh by 70% of the remaining triangles |
|8 |decimate the mesh by 80% of the remaining triangles |
|9 |decimate the mesh by 90% of the remaining triangles |
|0 |decimate the mesh by 99% of the remaining triangles |
|r |reset the mesh |
|- |decimate by one edge |
|space |toggle rotation |
