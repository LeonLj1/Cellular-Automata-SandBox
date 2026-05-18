#include "particles.h"
#include <algorithm>
#include <math.h>

using namespace std;

int grid[GRID_WIDTH][GRID_HEIGHT];

int surrounded(int x,int y, enum Cell particle){//is grid[x][y] surrounded with some particle type
    int n=0;//how many of particles surround grid[x][y]

    if(x-1>0 && grid[x-1][y]==particle) n++;
    if(x+1<GRID_WIDTH-1 && grid[x+1][y]==particle) n++;
    if(y-1>0 && grid[x][y-1]==particle) n++;
    if(y+1<GRID_HEIGHT-1 && grid[x][y+1]==particle) n++;

    return n;
}

int inCircle(int x,int y,int ccx,int ccy,int radius){//x,y coordinates of cell; ccx,ccy coordinates of circle center
    double d = sqrt((x-ccx)*(x-ccx)+(y-ccy)*(y-ccy));

    if(d<=radius) return 1;
    else return 0;
}

void draw(int x, int y, int brushSize,enum Cell particle){//function to make particles

    if(brushSize==0){
        grid[x][y]=particle;
        return;
    }

    for(int cy=y-brushSize;cy<=y+brushSize;cy++){
        for(int cx=x-brushSize;cx<=x+brushSize;cx++){
            int spawnParticle=!(rand()%20);
            if(inCircle(cx,cy,x,y,brushSize) && cx>=0 && cx<GRID_WIDTH && cy>=0 && cy<GRID_HEIGHT && spawnParticle){
                grid[cx][cy]=particle;
            }
        }
    }
}

void updateSand(int x,int y){
    if(y+1>=GRID_HEIGHT) return;

    int sandShift=!(rand()%15);

    if(grid[x][y+1]==EMPTY || (sandShift && grid[x][y+1]==WATER)){
        swap(grid[x][y],grid[x][y+1]);
    }else{
        
        int left=rand()%2;

        sandShift=!(rand()%15);

        if(left){
            if((x>0 && grid[x-1][y+1]==EMPTY) || (x>0 && sandShift && grid[x-1][y+1]==WATER)){
                swap(grid[x][y],grid[x-1][y+1]);
            }else{
                sandShift=!(rand()%15);
                if((x<GRID_WIDTH-1 && grid[x+1][y+1]==EMPTY) || (x<GRID_WIDTH-1 && sandShift && grid[x+1][y+1]==WATER)){
                    swap(grid[x][y],grid[x+1][y+1]);
                }
            }
        }else{
            sandShift=!(rand()%15);
            if((x<GRID_WIDTH-1 && grid[x+1][y+1]==EMPTY) || (x<GRID_WIDTH-1 && sandShift && grid[x+1][y+1]==WATER)){
                swap(grid[x][y],grid[x+1][y+1]);
            }else{
                sandShift=!(rand()%15);
                if((x>0 && grid[x-1][y+1]==EMPTY) || (x>0 && sandShift && grid[x-1][y+1]==WATER)){
                    swap(grid[x][y],grid[x-1][y+1]);
                }
            }
        }
    }
}

void updateWater(int x,int y){
    if(y+1 >= GRID_HEIGHT) return;

    if(grid[x][y+1]==EMPTY){
        swap(grid[x][y],grid[x][y+1]);
    }else{

        int dis=rand()%3+1;

        //if(grid[x+dis-1][y]!=EMPTY || grid[x+dis-2][y]!=EMPTY || grid[x-dis-1][y]!=EMPTY || grid[x-dis-2][y]!=EMPTY) return;

        if((x>dis-1 && grid[x-dis][y]==EMPTY && grid[x+1][y]!=EMPTY) || x+1>=GRID_WIDTH){
            swap(grid[x][y],grid[x-dis][y]);
            return;
        }else{
            if((x<GRID_WIDTH-dis && grid[x+dis][y]==EMPTY && grid[x-1][y]!=EMPTY) || x-1<=0){
                swap(grid[x][y],grid[x+dis][y]);
                return;
            } 
        }

        int left=rand()%2;

        if(left){
            if(x>dis-1 && grid[x-dis][y]==EMPTY){
                swap(grid[x][y],grid[x-dis][y]);
            }else{
                if(x<GRID_WIDTH-dis && grid[x+dis][y]==EMPTY){
                    swap(grid[x][y],grid[x+dis][y]);
                }
            }
        }else{
            if(x<GRID_WIDTH-dis && grid[x+dis][y]==EMPTY){
                swap(grid[x][y],grid[x+dis][y]);
            }else{
                if(x>dis-1 && grid[x-dis][y]==EMPTY){
                    swap(grid[x][y],grid[x-dis][y]);
                }
           }
        }
    }

}

void updateFire(int x,int y){

    int fireDesappear=!(rand()%10);
    int toSmoke=!(rand()%100);

    if(surrounded(x,y,WATER)){
        if(x-1>0 && grid[x-1][y]==WATER) grid[x-1][y]=SMOKE;
        if(x+1<GRID_WIDTH-1 && grid[x+1][y]==WATER) grid[x+1][y]=SMOKE;
        if(y-1>0 && grid[x][y-1]==WATER) grid[x][y-1]=SMOKE;
        if(y+1<GRID_HEIGHT-1 && grid[x][y+1]==WATER) grid[x][y+1]=SMOKE;

        grid[x][y]=SMOKE;
    }

    if(surrounded(x,y,WOOD)){
        toSmoke=!(rand()%7);
        fireDesappear=!(rand()%70);
    }

    if(fireDesappear){
        if(toSmoke){
            grid[x][y]=SMOKE;
        }else{
            grid[x][y]=EMPTY;
        }
        return;
    }

    if(y-1<=0) return;

    int spread=!(rand()%20);

    if(y-1>0 && grid[x][y-1]==EMPTY && spread){
        grid[x][y-1]=FIRE;
    }else{
        spread=!(rand()%20);
        if(x-1>0 && y-1>0 && grid[x-1][y-1]==EMPTY && spread){
            grid[x-1][y-1]=FIRE;
        }else{
            spread=!(rand()%20);
            if(x+1<GRID_WIDTH-1 && y-1>0 && grid[x+1][y-1]==EMPTY && spread){
                grid[x+1][y-1]=FIRE;
            }
        }
    }

}

void updateSmoke(int x,int y){

    int desappear=!(rand()%300);

    if(desappear){
        grid[x][y]=EMPTY;
        return;
    }

    if(y-1<=0) return;

    int toMove=!(rand()%5);

    if(y-1>0 && (grid[x][y-1]==EMPTY || grid[x][y-1]==WATER) && toMove){
        swap(grid[x][y],grid[x][y-1]);
    }else{
        if(x-1>0 && (grid[x-1][y-1]==EMPTY || grid[x-1][y-1]==WATER) && toMove){
            swap(grid[x][y], grid[x-1][y-1]);
        }else{
            if(x<GRID_WIDTH-1 && (grid[x+1][y-1]==EMPTY || grid[x+1][y-1]==WATER) && toMove){
                swap(grid[x][y], grid[x+1][y-1]);
            }
        }
    }
}

void updateWood(int x, int y){

    int getOnFire=!(rand()%30);

    if(surrounded(x,y,FIRE) && getOnFire){
        grid[x][y]=FIRE;
    }
    
}
