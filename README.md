# sudoku_solver
A Sudoku solver made in C++ using [SDL](https://www.libsdl.org/) for graphics.

## What is a Sudoku puzzle?
Sudoku puzzles have been around for a very long time, originating from France in the late 1800s. Modern sudoku (originally known as Number Place) is actually fairly new, first seen in American newspapers in 1979. Since then, sudoku puzzles have blown up in popularity and have since been modified into many different variations. [Find out more about Sudoku](https://en.wikipedia.org/wiki/Sudoku).

### Rules
The goal of any (9x9) sudoku puzzle is to find the missing numbers such that:
- Every row has the numbers 1-9 only once
- Every column has the numbers 1-9 only once
- Every square (the 9 3x3 regions of the board) has the numbers 1-9 only once

## Usage
Run "sudoku_solver.exe" to start the program.
Insert any known numbers by changing the selected square (yellow highlighted square) with the arrow keys or by clicking on it, and pressing the number on your keyboard.
An inserted number can be overridden by inserting a new number over it.
Then, press [ENTER] to start the solution (Note that the solution will not start if the numbers entered are not valid).

### Visual Cues
All numbers shown in a black font are numbers that you have entered.
While the board is being solved, blue numbers will appear showing numbers that are in the process of being solved.
A blue highlighted square will appear, indicating the current square the program is solving.
Once the solution is completed, the previously blue numbers will change to green, indicating the final solution. 

### Keybindings
- Arrow keys and left mouse button: move selected square
- Number keys 1-9: insert number into selected square
- [SHIFT]: show current solve speed
- [SHIFT] + 1-9: cap solve speed, (1-9 calculations per second, 9 being the fastest)
- [SHIFT] + 0: remove solve speed cap (fastest solution)
- [BACKSPACE]: remove number from selected square
- [ESCAPE]: clear the whole board
- [ENTER]: start solution
