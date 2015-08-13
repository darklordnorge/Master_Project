//
// This class holds the SDL functions to draw the map
// Created by stefan on 12/08/15.
//

#include "mapWindow.h"

MapWindow::MapWindow(){

}

void MapWindow::start(){
    if(!init()){
        printf("Failed to initialize!\n");
    }
    else{
        if(!load_media()){
            printf("Failed to load media!\n");
        }
        else{
            bool quit = false;

            SDL_Event e;

            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT){
                        quit = true;
                    }
                }
                draw_rectangle(0.2, 0.2, 0.1, 0.1);
            }
        }
    }

}

bool MapWindow::init(){
    bool success = true; // initilisation flag

    /*Initilaise SDL*/
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not be initialised! SDL error: %s\n", SDL_GetError());
    }
    else {
        //Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
            printf("Warning linear texture fildering not enabled");
        }

        /*Create Window*/
        g_window = SDL_CreateWindow("Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width,
                                    screen_height, SDL_WINDOW_SHOWN);
        if(g_window == NULL){
            printf("Window could not be created!SDL error: %s\n", SDL_GetError());
            success = false;
        }
        else{
            /*Create renderer for the window*/
            g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
            if(g_renderer == NULL){
                printf("Renderer could not be created! SDL error: %s\n", SDL_GetError());
                success = false;
            }
            else{
                /*Initialize renderer color*/
                SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                /*Initialize PNG image*/
                int imgFlag = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlag) & imgFlag)){
                    printf("SDL_image coult not initialized! SDL error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }

    }
    return success;
}

bool MapWindow::load_media() {
    /*loading success flag*/
    bool success = true;
//    nothing to load
    return success;
}

void MapWindow::close(){
    /*Destroy window*/
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    g_renderer = NULL;

    /*Quite SDL subsystems*/
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* MapWindow::load_texture(std::string path){
    //The final Texture
    SDL_Texture* new_texture = NULL;

    //load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if(loaded_surface == NULL){
        printf("Unable to load image %s ! SDL_image error: %s\n", path.c_str(), IMG_GetError());
    }
    else{
        //create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
        if(new_texture == NULL){
            printf("Unable to create texture from %s ! SDL error: %s\n", path.c_str(), SDL_GetError());
        }
        //delete old loaded surface
        SDL_FreeSurface(loaded_surface);
    }
    return new_texture;
}

void MapWindow::clear_screen() {
    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(g_renderer);
}

void MapWindow::draw_rectangle(int x_pos, int y_pos, int width, int height) {
    SDL_Rect rect = {x_pos, y_pos, width, height};
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0xff, 0x00, 0xff); //red colour
    SDL_RenderFillRect(g_renderer, &rect);
}

void MapWindow::update_screen(){
    SDL_RenderPresent(g_renderer);
}






