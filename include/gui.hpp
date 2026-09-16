#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Texture* texture;
extern SDL_Event event;
extern TTF_Font* Font;
extern SDL_Color textColor;

class Button{
    protected:
        int x,y;
        string text;
        int width,height;
        int thickness = 2;//button edge thickness
    public:

        bool isClicked = false;

        Button(int x,int y,int width,int height,string text){
            this -> x = x;
            this -> y = y;
            this -> width = width;
            this -> height = height;
            this -> text = text;
            unclicked();
            unclickedText();
        }

        virtual void onClick(){}

        void show(){
            unclicked();
            unclickedText();
        }

        void unclickedText(){
            int textWidth;
            int textHeight;
            TTF_SizeText(Font,text.c_str(),&textWidth, &textHeight);
            SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text.c_str(), textColor);

            SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect Message_Rect;

            Message_Rect.x = x + (width - textWidth)/2 + thickness;
            Message_Rect.y = y + (height - textHeight)/2 + thickness;
            Message_Rect.w = textWidth;
            Message_Rect.h = textHeight;

            SDL_RenderCopy(renderer, Message, NULL, &Message_Rect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(Message);
        }

        void clickedText(){
            int textWidth;
            int textHeight;
            TTF_SizeText(Font,text.c_str(),&textWidth, &textHeight);
            SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text.c_str(), {0, 0, 0});

            SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect Message_Rect;

            Message_Rect.x = x + (width - textWidth)/2 + thickness;
            Message_Rect.y = y + (height - textHeight)/2 + thickness;
            Message_Rect.w = textWidth;
            Message_Rect.h = textHeight;

            SDL_RenderCopy(renderer, Message, NULL, &Message_Rect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(Message);
        }

        virtual void unclicked(){
            SDL_SetRenderDrawColor(renderer, 255, 255,255, 255);

            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.h = thickness;
            rect.w = width;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x;
            rect.y = y + thickness;
            rect.w = thickness;
            rect.h = height - 2 * thickness;
            SDL_RenderFillRect(renderer, &rect);
            
            rect.x = x;
            rect.y = y + height - thickness;
            rect.w = width;
            rect.h = thickness;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width - thickness;
            rect.y = y + thickness;
            rect.w = thickness;
            rect.h = height - 2 * thickness;
            SDL_RenderFillRect(renderer, &rect);
        }

        virtual void clicked(){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_Rect rect;
            rect.x = x + thickness;
            rect.y = y + thickness;
            rect.h = height - 2*thickness;
            rect.w = width - 2*thickness;
            SDL_RenderFillRect(renderer, &rect);
        }

        bool inButton(int x,int y){
            if(x >= this -> x && x<= this -> x + width && y >= this -> y && y <= this -> y + height) return true;
            return false;
        }

        virtual void click(){
            clicked();
            clickedText();
            SDL_RenderPresent(renderer);

            while(SDL_PollEvent(&event) || event.type != SDL_MOUSEBUTTONUP);
            unclicked();
            unclickedText();
            SDL_RenderPresent(renderer);

            clear();

            onClick();
        }

        void clear(){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_Rect rect;

            rect.x = x;
            rect.y = y;
            rect.h = height;
            rect.w = width;
            SDL_RenderCopy(renderer, texture, &rect, &rect);
            SDL_RenderPresent(renderer);
        }
};

class MenuButton : public Button {
    public:
        MenuButton(int x,int y,int width,int height) : Button(x, y, width, height, "menu"){}

        void unclicked() override {
            SDL_SetRenderDrawColor(renderer, 255, 255,255, 255);

            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.h = thickness;
            rect.w = width;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x;
            rect.y = y + thickness;
            rect.w = thickness;
            rect.h = height - 2 * thickness;
            SDL_RenderFillRect(renderer, &rect);
            
            rect.x = x;
            rect.y = y + height - thickness;
            rect.w = width;
            rect.h = thickness;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width - thickness;
            rect.y = y + thickness;
            rect.w = thickness;
            rect.h = height - 2 * thickness;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width/7;
            rect.y = y + height/3 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width/7;
            rect.y = y + height/2 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width/7;
            rect.y = y + 2 * height/3 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            SDL_RenderPresent(renderer);
        }

        void clicked() override {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_Rect rect;
            rect.x = x + thickness;
            rect.y = y + thickness;
            rect.h = height - 2*thickness;
            rect.w = width - 2*thickness;
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                

            rect.x = x + width/7;
            rect.y = y + height/3 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width/7;
            rect.y = y + height/2 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            rect.x = x + width/7;
            rect.y = y + 2 * height/3 - height/20;
            rect.h = height/15;
            rect.w = 5 * width/7;
            SDL_RenderFillRect(renderer, &rect);

            SDL_RenderPresent(renderer);
        }
        
        void onClick() override {
            isClicked = false;
        }

        void click() override{
            clicked();

            while(SDL_PollEvent(&event) || event.type != SDL_MOUSEBUTTONUP);
            unclicked();

            onClick();
        }
};

class ControlsButton : public Button {
    public:

        ControlsButton(int x, int y, int width, int height, string text) : Button(x, y, width, height, text){}

        void onClick() override{
            bool running = true;



            while(running){
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_ESCAPE) running = false;
                }
            }
        }
};

class MapModeButton : public Button {
    public:

        MapModeButton(int x, int y, int width, int height, string text) : Button(x, y, width, height, text){}

        void onClick() override{
            bool running = true;



            while(running){
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_ESCAPE) running = false;
                }
            }
        }
};

class LoadButton : public Button {
    public:

        LoadButton(int x, int y, int width, int height, string text) : Button(x, y, width, height, text){}

        void onClick() override{
            bool running = true;



            while(running){
                while(SDL_PollEvent(&event)){
                    if(event.key.keysym.sym == SDLK_ESCAPE) running = false;
                }
            }
        }
};

#endif