#include "solver.h"
#include "include/queue.h"
#include "mouse.h"

// Initializing variables
Cell decMaze[16][16]; // y, x
int lastPosition[2] = {-1, -1};
int lastHeading = NORTH;
int goal[2] = {-1, -1};
enum State { GOING, BACKING, NOTHING };
enum State turn = GOING;

Action solver(Mouse *mouse) { return floodFill(mouse); }

void printMaze(Cell maze[16][16], int size) {
  for (int x = size - 1; x >= 0; x--) {
    for (int y = 0; y < size; y++)
      printf("%3d", maze[x][y].distance);
    printf("\n");
  }
  printf("\n");
}

// Going from start to finish and finish to start holding the matrix.
Action floodFill(Mouse *mouse) {
  int newCell = lastPosition[0] != mouse->x || lastPosition[1] != mouse->y;
  int newHeading = lastHeading != mouse->heading;
  int size = mouse->maze->size;
  int x = mouse->x;
  int y = mouse->y;
  int currentHeandingIndex = mouse->heading;

  goal[0] = (turn == GOING) ? mouse->maze->finish_pos[0] : 0;
  goal[1] = (turn == GOING) ? mouse->maze->finish_pos[1] : 0;

  if ((newCell || newHeading)) {
    switch (mouse->heading) {
    case NORTH:
      if (getFrontReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      break;

    case SOUTH:
      if (getFrontReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      break;

    case EAST:
      if (getFrontReading(mouse)) {
        decMaze[y][x].E = 1;
        if (x + 1 < size)
          decMaze[y][x + 1].W = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      break;

    case WEST:
      if (getFrontReading(mouse)) {
        decMaze[y][x].W = 1;
        if (x - 1 >= 0)
          decMaze[y][x - 1].E = 1;
      }
      if (getLeftReading(mouse)) {
        decMaze[y][x].S = 1;
        if (y - 1 >= 0)
          decMaze[y - 1][x].N = 1;
      }
      if (getRightReading(mouse)) {
        decMaze[y][x].N = 1;
        if (y + 1 < size)
          decMaze[y + 1][x].S = 1;
      }
      break;
    }

    floodFillAlg(size, goal);

    if (turn == GOING && mouse->x == mouse->maze->finish_pos[0] &&
        mouse->y == mouse->maze->finish_pos[1]) {
      turn = BACKING;
    }

    // Cuando vuelve al inicio, pasa a GOING
    if (turn == BACKING && mouse->x == 0 && mouse->y == 0) {
      turn = GOING;
    }

    // Determine best move based on current heading and flood fill values
    int currentDistance = decMaze[y][x].distance;
    Action bestAction = IDLE;
    int bestDistance = currentDistance;

    // Check available directions and map them to mouse actions
    switch (mouse->heading) {
    case NORTH:
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y + 1][x].distance;
      }
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y][x - 1].distance;
      }
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y][x + 1].distance;
      }
      break;

    case SOUTH:
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y - 1][x].distance;
      }
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y][x + 1].distance;
      }
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y][x - 1].distance;
      }
      break;

    case EAST:
      if (!decMaze[y][x].E && x + 1 < size &&
          decMaze[y][x + 1].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y][x + 1].distance;
      }
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y + 1][x].distance;
      }
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y - 1][x].distance;
      }
      break;

    case WEST:
      if (!decMaze[y][x].W && x - 1 >= 0 &&
          decMaze[y][x - 1].distance < bestDistance) {
        bestAction = FORWARD;
        bestDistance = decMaze[y][x - 1].distance;
      }
      if (!decMaze[y][x].S && y - 1 >= 0 &&
          decMaze[y - 1][x].distance < bestDistance) {
        bestAction = LEFT;
        bestDistance = decMaze[y - 1][x].distance;
      }
      if (!decMaze[y][x].N && y + 1 < size &&
          decMaze[y + 1][x].distance < bestDistance) {
        bestAction = RIGHT;
        bestDistance = decMaze[y + 1][x].distance;
      }
      break;
    }

    if (bestAction == IDLE && turn != NOTHING) {
      bestAction = LEFT;
    }
    printMaze(decMaze, 16);

    return bestAction;
  }
  return IDLE;
}

void checkNorth(Node *c, Queue *q, int MazeSize, int currentDistance) {
  if (c->y + 1 < MazeSize && !decMaze[c->y][c->x].N &&
      !decMaze[c->y + 1][c->x].visited) {
    enqueue(q, c->x, c->y + 1);
    decMaze[c->y + 1][c->x].distance = currentDistance + 1;
    decMaze[c->y + 1][c->x].visited = 1;
  }
}

void checkSouth(Node *c, Queue *q, int MazeSize, int currentDistance) {
  if (c->y - 1 >= 0 && !decMaze[c->y][c->x].S &&
      !decMaze[c->y - 1][c->x].visited) {
    enqueue(q, c->x, c->y - 1);
    decMaze[c->y - 1][c->x].distance = currentDistance + 1;
    decMaze[c->y - 1][c->x].visited = 1;
  }
}

void checkEast(Node *c, Queue *q, int MazeSize, int currentDistance) {
  if (c->x + 1 < MazeSize && !decMaze[c->y][c->x].E &&
      !decMaze[c->y][c->x + 1].visited) {
    enqueue(q, c->x + 1, c->y);
    decMaze[c->y][c->x + 1].distance = currentDistance + 1;
    decMaze[c->y][c->x + 1].visited = 1;
  }
}

void checkWest(Node *c, Queue *q, int MazeSize, int currentDistance) {
  if (c->x - 1 >= 0 && !decMaze[c->y][c->x].W &&
      !decMaze[c->y][c->x - 1].visited) {
    enqueue(q, c->x - 1, c->y);
    decMaze[c->y][c->x - 1].distance = currentDistance + 1;
    decMaze[c->y][c->x - 1].visited = 1;
  }
}

void floodFillAlg(int MazeSize, int startPoint[2]) {
  for (int y = 0; y < MazeSize; ++y) {
    for (int x = 0; x < MazeSize; ++x) {
      decMaze[y][x].distance = -1;
      decMaze[y][x].visited = 0;
    }
  }
  Queue *q = initQueue();

  enqueue(q, startPoint[0], startPoint[1]);

  decMaze[startPoint[1]][startPoint[0]].distance = 0;
  decMaze[startPoint[1]][startPoint[0]].visited = 1;

  while (!isEmpty(q)) {
    Node *c = dequeue(q);
    int currentDistance = decMaze[c->y][c->x].distance;
    checkNorth(c, q, MazeSize, currentDistance);
    checkSouth(c, q, MazeSize, currentDistance);
    checkEast(c, q, MazeSize, currentDistance);
    checkWest(c, q, MazeSize, currentDistance);
    free(c);
  }
};
