#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <vector>

const int WIDTH=900;
const int HEIGHT=600;

const int CELL_SIZE=4;

const int GRID_WIDTH=WIDTH/CELL_SIZE;
const int GRID_HEIGHT=HEIGHT/CELL_SIZE;

enum Cell{
    EMPTY,
    SAND,
    WATER
};

std::vector<Cell> grid(GRID_WIDTH*GRID_HEIGHT,EMPTY);

inline int idx(int x,int y){
    return y*GRID_WIDTH+x;
}

void updateSand(int x,int y){
    if(y+1>=GRID_HEIGHT) return;

    if(grid[idx(x,y+1)]==EMPTY){
        std::swap(grid[idx(x,y)],grid[idx(x,y+1)]);
    }else{
        
        int left=rand()%2;

        if(left){
            if(x>0 && grid[idx(x-1,y+1)]==EMPTY){
                std::swap(grid[idx(x,y)],grid[idx(x-1,y+1)]);
            }else{
                if(x<GRID_WIDTH-1 && grid[idx(x+1,y+1)]==EMPTY){
                    std::swap(grid[idx(x,y)],grid[idx(x+1,y+1)]);
                }
            }
        }else{
            if(x<GRID_WIDTH-1 && grid[idx(x+1,y+1)]==EMPTY){
                std::swap(grid[idx(x,y)],grid[idx(x+1,y+1)]);
            }else{
                if(x>0 && grid[idx(x-1,y+1)]==EMPTY){
                    std::swap(grid[idx(x,y)],grid[idx(x-1,y+1)]);
                }
            }
        }
    }
}

void updateWater(int x,int y){
    if(y+1 >= GRID_HEIGHT) return;

    if(grid[idx(x,y+1)]==EMPTY){
        std::swap(grid[idx(x,y)],grid[idx(x,y+1)]);
    }else{
        int left=rand()%2;

        if(left){
            if(x>0 && grid[idx(x-1,y)]==EMPTY){
                std::swap(grid[idx(x,y)],grid[idx(x-1,y)]);
            }else{
                if(x<GRID_WIDTH-1 && grid[idx(x+1,y)]==EMPTY){
                    std::swap(grid[idx(x,y)],grid[idx(x+1,y)]);
                }
            }
        }else{
            if(x<GRID_WIDTH-1 && grid[idx(x+1,y)]==EMPTY){
                std::swap(grid[idx(x,y)],grid[idx(x+1,y)]);
            }else{
                if(x>0 && grid[idx(x-1,y)]==EMPTY){
                    std::swap(grid[idx(x,y)],grid[idx(x-1,y)]);
                }
            }

        }
    }
    
}

int main(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        std::cout<<"SDL intit failed!!!\n";
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

                    switch(ch-'0'){
                        case 0: particle=0; break;
                        case 1: particle=1; break;
                        case 2: particle=2; break;
                        default: particle=1; break;
                    }
                }
            }
        }

        int mx,my;
        Uint32 mouse = SDL_GetMouseState(&mx,&my);

        int gx=mx/CELL_SIZE, gy=my/CELL_SIZE;

        if(mouse & SDL_BUTTON(SDL_BUTTON_LEFT)){
            if(gx>=0 && gx<GRID_WIDTH && gy>=0 && gy<GRID_HEIGHT){
                switch(ch-'0'){
                    case 0: grid[idx(gx,gy)]=EMPTY; break;
                    case 1: grid[idx(gx,gy)]=SAND; break;
                    case 2: grid[idx(gx,gy)]=WATER; break;
                    default: grid[idx(gx,gy)]=SAND; break;
                }
            }
        }

        for(int y=GRID_HEIGHT-1;y>=0;y--){
            for(int x=0;x<GRID_WIDTH;x++){
                if(grid[idx(x,y)]==SAND){
                    updateSand(x,y);
                }else{
                    if(grid[idx(x,y)]==WATER){
                        updateWater(x,y);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for(int y=0;y<GRID_HEIGHT;y++){
            for(int x=0;x<GRID_WIDTH;x++){
                if(grid[idx(x,y)]==SAND){
                    SDL_Rect r = {
                        x*CELL_SIZE,
                        y*CELL_SIZE,
                        CELL_SIZE,
                        CELL_SIZE
                    };
                    SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255);
                    SDL_RenderFillRect(renderer, &r);
                }else{
                    if(grid[idx(x,y)]==WATER){
                        SDL_Rect r = {
                            x*CELL_SIZE,
                            y*CELL_SIZE,
                            CELL_SIZE,
                            CELL_SIZE
                        };

                        SDL_SetRenderDrawColor(renderer, 0, 105, 148, 255);
                        SDL_RenderFillRect(renderer, &r);
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
