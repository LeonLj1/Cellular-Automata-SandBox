#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "particles.h"

using namespace std;

Uint32 cellColor(int x, int y){
    switch(grid[x][y]){
        case SAND:  return 0xC2B280FF;
        case WATER: return 0x006994FF;
        case STONE: return 0x808080FF;
        case WOOD:  return 0x9C7740FF;
        case SMOKE: return 0xC8C8C8FF;
        case FIRE:
            switch(surrounded(x,y,FIRE)){
                case 0: return 0xC80000FF;
                case 1: return 0xE86507FF;
                case 2: return 0xEBB328FF;
                case 3: return 0xFCE142FF;
                case 4: return 0xFFFFFFFF;
            }
        default: return 0x000000FF;
    }
}

int main(){

    int brushSize=100;

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

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        GRID_WIDTH,
        GRID_HEIGHT
    );

    int running=1;

    SDL_Event event;

    char ch,chbs='0',chp='s';//chbs - char for brush size, chp - char for particle

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

        if(chp == 'm'){
            chp='s';
            bool mapMode=true;
            char chbs='0', chp='s';

            while(mapMode){

                while(SDL_PollEvent(&event)){
                    if(event.type == SDL_TEXTINPUT){
                        char ch=event.text.text[0];
                        if(ch>='0' && ch<='9'){
                            chbs=ch;
                        }else{
                            chp=ch;
                        }
                    }else{
                        if(event.type == SDL_QUIT){
                            mapMode=false;
                            running=0;
                        }
                    }
                }

                if(chp=='r'){
                    memset(grid, 0, sizeof(grid));
                    chp='s';
                }

                if(chp=='q'){
                    mapMode=false;
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

                Uint32* pixels;
                int pitch;

                SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
                for(int y = 0; y < GRID_HEIGHT; y++){
                    for(int x = 0; x < GRID_WIDTH; x++){
                        pixels[y * (pitch/4) + x] = cellColor(x, y);
                    }
                }
                SDL_UnlockTexture(texture);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);

                if(chp == 'a'){
                    chp='q';
                    char mapName[100]="";

                    printf("Type the name of map you want to save:\n");
                    scanf("%s",mapName);

                    char directory[200]="maps/";
                    strcat(directory,mapName);

                    FILE* mapFile = fopen(directory,"w");
                    
                    for(int y=0;y<GRID_HEIGHT;y++){
                        for(int x=0;x<GRID_WIDTH;x++){
                            fprintf(mapFile,"%d ",grid[x][y]);
                        }
                        fprintf(mapFile,"\n");
                    }
                    fclose(mapFile);
                }

            }
            
        }

        if(chp == 'l'){
            chp='s';

            char mapName[100]="";
            printf("Type the name of map you want to load:\n");
            scanf("%s",mapName);

            char directory[200]="maps/";
            strcat(directory,mapName);

            FILE* mapFile = fopen(directory,"r");
            for(int y=0;y<GRID_HEIGHT;y++){
                for(int x=0;x<GRID_WIDTH;x++){
                    fscanf(mapFile,"%d ",&grid[x][y]);
                }
            }
            fclose(mapFile);
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

                switch (grid[x][y]){
                    case SAND: updateSand(x,y); break;
                    case WATER: updateWater(x,y); break;
                    case FIRE: updateFire(x,y); break;
                    case SMOKE: updateSmoke(x,y); break;
                    case WOOD: if(surrounded(x,y,WOOD)==4);
                               else updateWood(x,y); break;
                }
            }
        }

        Uint32* pixels;

        int pitch;

        SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

        for(int y = 0; y < GRID_HEIGHT; y++){
            for(int x = 0; x < GRID_WIDTH; x++){
                pixels[y * (pitch/4) + x] = cellColor(x, y);
            }
        }

        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(3);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
