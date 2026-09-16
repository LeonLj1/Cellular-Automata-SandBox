#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
#include <particles.hpp>
#include <gui.hpp>

using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Event event;
TTF_Font* Font = nullptr;
SDL_Color textColor = {255, 255, 255};

int main(int argc, char* argv[]){

    int brushSize=100;

    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        cout<<"SDL intit failed!!!\n";
        return -1;
    }

    if(TTF_Init() == -1){
        cout << "TTF Failed!!!: " << TTF_GetError() << endl;
        return -1;
    }

    window=SDL_CreateWindow(
        "Falling sand",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,HEIGHT,
        0
    );

    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        GRID_WIDTH,
        GRID_HEIGHT
    );

    Font = TTF_OpenFont("assets/Font.ttf", 24);

    if(Font == nullptr) {
        cout << "Failed to open Font. SDL_Error: " << TTF_GetError() << "\n";
    }

    MenuButton menu(10,10,50,30);

    int running=1;

    char ch,chbs='0',chp='s';//chbs - char for brush size, chp - char for particle

    while(running){

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

                    cout << "Type the name of map you want to save:\n";
                    cin >> mapName;

                    char directory[200]="assets/maps/";
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
            cout << "Type the name of map you want to load:\n";
            cin >> mapName;

            char directory[200]="assets/maps/";
            strcat(directory,mapName);

            FILE* mapFile = fopen(directory,"r");

            if( mapFile == NULL){
                while(mapFile == NULL){
                cout << "Invalid map!!!" << endl;
                cout << "Type the name of map you want to load:\n";
                cin >> mapName;

                char directory[200]="assets/maps/";
                strcat(directory,mapName);

                mapFile = fopen(directory,"r");
                }
            }

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

        if(mouse && SDL_BUTTON(SDL_BUTTON_LEFT) && menu.inButton(mx,my)){
            menu.click();
            menu.isClicked = true;

            SDL_RenderCopy(renderer, texture, NULL, NULL);

            ControlsButton controls(300, 270, 200, 80, "Controls");
            MapModeButton MapMode(300, 360, 200, 80, "Map Mode");
            LoadButton load(300, 450, 200, 80, "Load Map");

            menu.unclicked();

            while(menu.isClicked){

                while(SDL_PollEvent(&event)){
                    if(event.type == SDL_QUIT){
                        menu.isClicked = false;
                        running = false;
                    }else{
                        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                            menu.isClicked = false;
                        }else{
                            if(event.type == SDL_MOUSEBUTTONDOWN){
                                int mx,my;
                                mouse = SDL_GetMouseState(&mx, &my);
                                if(menu.inButton(mx,my)){
                                    menu.click();
                                    continue;
                                }
                                if(controls.inButton(mx,my)){ 
                                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                                    MapMode.clear();
                                    load.clear();
                                    controls.click();
                                    controls.show();
                                    MapMode.show();
                                    load.show();
                                    menu.unclicked();
                                    continue;
                                }
                                if(MapMode.inButton(mx,my)){
                                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                                    controls.clear();
                                    load.clear();
                                    MapMode.click();
                                    controls.show();
                                    MapMode.show();
                                    load.show();
                                    menu.unclicked();
                                    continue;
                                }
                                if(load.inButton(mx,my)){
                                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                                    controls.clear();
                                    MapMode.clear();
                                    load.click();
                                    controls.show();
                                    MapMode.show();
                                    load.show();
                                    menu.unclicked();
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }

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
                    case WOOD: if(surrounded(x,y,WOOD)==4) break;
                                updateWood(x,y); break;
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
        menu.unclicked();
        SDL_Delay(3);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
