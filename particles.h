#ifndef PARTICLES_H
#define PARTICLES_H

const int WIDTH = 1920;
const int HEIGHT = 1080;

const int CELL_SIZE = 4;
const int GRID_WIDTH = WIDTH/CELL_SIZE;
const int GRID_HEIGHT = HEIGHT/CELL_SIZE;

enum Cell { 
    EMPTY, 
    SAND, 
    WATER, 
    STONE, 
    WOOD, 
    FIRE, 
    SMOKE 
};

extern int grid[GRID_WIDTH][GRID_HEIGHT];

int surrounded(int x, int y, enum Cell particle);
int inCircle(int x, int y, int ccx, int ccy, int radius);
void draw(int x, int y, int brushSize, enum Cell particle);
void updateSand(int x, int y);
void updateWater(int x, int y);
void updateFire(int x, int y);
void updateSmoke(int x, int y);
void updateWood(int x, int y);

#endif