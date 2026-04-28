#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>

using namespace std;

const int WIDTH=900;
const int HEIGHT=600;

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

    if(y-1>0 && grid[x][y-1]==EMPTY && toMove){
        swap(grid[x][y],grid[x][y-1]);
    }else{
        if(x-1>0 && grid[x-1][y-1]==EMPTY && toMove){
            swap(grid[x][y], grid[x-1][y-1]);
        }else{
            if(x<GRID_WIDTH-1 && grid[x+1][y-1]==EMPTY && toMove){
                swap(grid[x][y], grid[x+1][y-1]);
            }
        }
    }
}

void updateWood(int x, int y){
    
}

int main(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        cout<<"SDL intit failed!!!\n";
        return -1;
    }

    SDL_Window* window=SDL_CreateWindow(
        "Falling sand",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,HEIGHT,
        0
    );

    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    int running=1;

    SDL_Event event;

    while(running){

        char ch;
        int particle;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) running=0;
            else{
                if(event.type == SDL_TEXTINPUT){
                    ch=event.text.text[0];   
                }
            }
        }

        int mx,my;
        Uint32 mouse = SDL_GetMouseState(&mx,&my);

        int gx=mx/CELL_SIZE, gy=my/CELL_SIZE;

        if(mouse & SDL_BUTTON(SDL_BUTTON_LEFT)){
            if(gx>=0 && gx<GRID_WIDTH && gy>=0 && gy<GRID_HEIGHT){
                switch(ch-'0'){
                    case 0: grid[gx][gy]=EMPTY; break;
                    case 1: grid[gx][gy]=SAND; break;
                    case 2: grid[gx][gy]=WATER; break;
                    case 3: grid[gx][gy]=STONE; break;
                    case 4: grid[gx][gy]=WOOD; break;
                    case 5: grid[gx][gy]=FIRE; break;
                    case 6: grid[gx][gy]=SMOKE; break;
                    default: grid[gx][gy]=SAND; break;
                 }
            }
        }

        for(int y=GRID_HEIGHT-1;y>=0;y--){

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
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for(int y=0;y<GRID_HEIGHT;y++){
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
                                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
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
