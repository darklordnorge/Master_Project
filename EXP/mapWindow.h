//
// Created by stefan on 12/08/15.
//

#ifndef PROJECT_MAPWINDOW_H
#define PROJECT_MAPWINDOW_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <SDL2/SDL_image.h>

class MapWindow {
    private:
        static const int screen_width = 640;
        static const int screen_height = 480;
        SDL_Window* g_window = NULL;
        SDL_Renderer* g_renderer = NULL;



    public:
        MapWindow();
        bool init();
        bool load_media();
        void close(void);
        SDL_Texture* load_texture(std::string path);
        void clear_screen(void);
        void draw_rectangle(int x_pos, int y_pos, int width, int height);
        void update_screen(void);
        void start(void);


};

#endif //PROJECT_MAPWINDOW_H
