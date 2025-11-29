#ifndef SOLVER_H
#define SOLVER_H

#include "./include/stack.h"
#include "mouse.h"

typedef struct Cell {
  int distance;
  int N, S, W, E;
  int visited;
} Cell;

Action solver(Mouse *mouse);
Action obstacleAvoider(Mouse *mouse);
NodeStack *getBestPath(Mouse *mouse, Cell maze[16][16]);
Action sendPath(NodeStack **stack);
Action leftWallFollower(Mouse *mouse);
Action floodFill(Mouse *mouse);
void floodFillAlg(int MazeSize, int startPoint[2]);

#endif
