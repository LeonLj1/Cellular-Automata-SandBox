#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int WIDTH=1920;
const int HEIGHT=1080;

const int CELL_SIZE=4;

const int GRID_WIDTH=WIDTH/CELL_SIZE;
const int GRID_HEIGHT=HEIGHT/CELL_SIZE;

enum Cell{
    EMPTY,
    SAND,
    WATER,
    STONE,
    WOOD,
    FIRE,
    SMOKE
};

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

    for(int cx=x-brushSize;cx<=x+brushSize;cx++){
        for(int cy=y-brushSize;cy<=y+brushSize;cy++){
            if(inCircle(cx,cy,x,y,brushSize) && cx>=0 && cx<GRID_WIDTH && cy>=0 && cy<GRID_HEIGHT){
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

    int desappear=!(rand()%100);

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

int main(){

    int brushSize=1;

    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        cout<<"SDL intit failed!!!\n";
        return -1;
    }

    SDL_Window* window=SDL_CreateWindow(
        "Falling sand",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    int running=1;

    SDL_Event event;

    char ch,chbs='0',chp;//chbs - char for brush size, chp - char for particle

    while(running){

        int particle;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) running=0;
            else{
                if(event.type == SDL_TEXTINPUT){
                    ch=event.text.text[0];   
                    if(ch>='0' && ch<='9'){
                        chbs=ch;
                    }else{
                        chp=ch;
                    }
                }
            }
        }

        if(chp=='r'){
            memset(grid, 0, sizeof(grid));
            chp='s';
        }

        brushSize=chbs-'0';

        int mx,my;
        Uint32 mouse = SDL_GetMouseState(&mx,&my);

        int gx=mx/CELL_SIZE, gy=my/CELL_SIZE;

        if(mouse & SDL_BUTTON(SDL_BUTTON_LEFT)){
            if(gx>=0 && gx<GRID_WIDTH && gy>=0 && gy<GRID_HEIGHT){//draws particles where mouse points
                switch(chp){
                    case 'e': draw(gx,gy,brushSize,EMPTY); break;
                    case 's': draw(gx,gy,brushSize,SAND); break;
                    case 'w': draw(gx,gy,brushSize,WATER); break;
                    case 'k': draw(gx,gy,brushSize,STONE); break;
                    case 'd': draw(gx,gy,brushSize,WOOD); break;
                    case 'f': draw(gx,gy,brushSize,FIRE); break;
                    case 'p': draw(gx,gy,brushSize,SMOKE); break;
                    default: draw(gx,gy,brushSize,SAND); break;
                 }
            }
        }

        for(int y=GRID_HEIGHT-1;y>=0;y--){// for loop for updateing particles

            int leftToRight = rand() % 2;

            for(int i=0;i<GRID_WIDTH;i++){
                int x = leftToRight ? i : GRID_WIDTH-1-i;

                if(grid[x][y]==SAND){
                    updateSand(x,y);
                }else{
                    if(grid[x][y]==WATER){
                        updateWater(x,y);
                    }else{
                        if(grid[x][y]==FIRE){
                            updateFire(x,y);
                        }else{
                            if(grid[x][y]==SMOKE){
                                updateSmoke(x,y);
                            }else{
                                if(grid[x][y]==WOOD){
                                    if(surrounded(x,y,WOOD)==4);
                                    else{
                                        updateWood(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for(int y=0;y<GRID_HEIGHT;y++){//for loop for rendering particles
            for(int x=0;x<GRID_WIDTH;x++){
                if(grid[x][y]==SAND){
                    SDL_Rect r = {
                        x*CELL_SIZE,
                        y*CELL_SIZE,
                        CELL_SIZE,
                        CELL_SIZE
                    };

                    SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255);
                    SDL_RenderFillRect(renderer, &r);
                }else{
                    if(grid[x][y]==WATER){
                        SDL_Rect r = {
                            x*CELL_SIZE,
                            y*CELL_SIZE,
                            CELL_SIZE,
                            CELL_SIZE
                        };

                        SDL_SetRenderDrawColor(renderer, 0, 105, 148, 255);
                        SDL_RenderFillRect(renderer, &r);
                    }else{
                        if(grid[x][y]==STONE){
                            SDL_Rect r = {
                                x*CELL_SIZE,
                                y*CELL_SIZE,
                                CELL_SIZE,
                                CELL_SIZE
                            };

                            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                            SDL_RenderFillRect(renderer, &r);
                        }else{
                            if(grid[x][y]==WOOD){
                                SDL_Rect r={
                                    x*CELL_SIZE,
                                    y*CELL_SIZE,
                                    CELL_SIZE,
                                    CELL_SIZE
                                };
                                SDL_SetRenderDrawColor(renderer, 156, 119, 64, 255);
                                SDL_RenderFillRect(renderer, &r);
                            }else{
                                if(grid[x][y]==FIRE){
                                    SDL_Rect r={
                                        x*CELL_SIZE,
                                        y*CELL_SIZE,
                                        CELL_SIZE,
                                        CELL_SIZE
                                    };

                                    switch(surrounded(x,y,FIRE)){//gives textures to fire
                                        case 0:SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);break;
                                        case 1:SDL_SetRenderDrawColor(renderer, 232, 101, 7, 255);break;
                                        case 2:SDL_SetRenderDrawColor(renderer, 235, 179, 40, 255);break;
                                        case 3:SDL_SetRenderDrawColor(renderer, 252, 225, 66, 255);break;
                                        case 4: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);break;
                                    }

                                    SDL_RenderFillRect(renderer, &r);
                                }else{
                                    if(grid[x][y]==SMOKE){
                                        SDL_Rect r={
                                            x*CELL_SIZE,
                                            y*CELL_SIZE,
                                            CELL_SIZE,
                                            CELL_SIZE
                                        };
                                        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                                        SDL_RenderFillRect(renderer, &r);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(2);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
