# PuzzleSolver
## Literature
[Puzzle Project](http://www.cs.virginia.edu/~robins/cs6161/PuzzleProject.pdf)

[Knuth's Algorithm X for exact cover problem](https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X)

[Dancing Links](https://en.wikipedia.org/wiki/Dancing_Links)

## Use dancing Links algorithm to solve pentominos
Assume total number of tiles is _k_


## Cairo Graphics Library
This project intends to use Cairo as a light-weight graphics library for demonstrating the output.
This project will automatically use x86 version of Cairo library. For most case this will just work fine.
If users desire to use x64 version of Cairo, go to the following two settings in Visual Studio and change "x86" to "x64" inside correpondingly:
```
Project -> PuzzleSolverProperties -> Linker -> General -> Additional Library Directories
Project -> PuzzleSolverProperties -> Build Events ->  Post-Build Event -> Command Line
```
