//
// Created by stefan on 12/08/15.
//

#ifndef PROJECT_MAPWINDOW_H
#define PROJECT_MAPWINDOW_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>

class mapWindow{
    protected:
        const int screen_width = 640;
        const int screen_height = 480;
        SDL_Texture* load_textture(std::string path);
        SDL_Window* g_window = NULL;
        SDL_Renderer* g_renderer = NULL;


    public:
        bool init();
        bool load_media();

};

#endif //PROJECT_MAPWINDOW_H
