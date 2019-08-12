/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../../SDL_internal.h"

#include "../SDL_sysvideo.h"

#include "SDL_orbitalwindow.h"
#include "SDL_orbitalvideo.h"
#include <orbital.h>
#include <assert.h>

/* Define a path to window's ORBITAL data */
#ifdef __cplusplus
extern "C" {
#endif

const char* DEFAULT_ORB_WINDOW_TITLE = "SDL Orbital window";

int init_window(SDL_Window* window, SDL_VideoData* video_data, Uint32 flags) {
    const char* title = window->title != NULL ? window->title : DEFAULT_ORB_WINDOW_TITLE;
    void* orb_window = NULL;
    fprintf(stderr, "Initializing orbital window at %d,%d with size %dx%d with flags %d\n", window->x, window->y, window->w, window->h, flags);
    orb_window = orb_window_new_flags(window->x, window->y, window->w, window->h, title, flags);
    if (orb_window != NULL) {
        video_data->orb_window = orb_window;
        //fprintf(stderr, "Orbital window creation success\n");
    } else {
        fprintf(stderr, "Orbital window creation failed\n");
    }

    return video_data->orb_window != NULL ? 0 : -1;
}

int ORBITAL_CreateWindow(_THIS, SDL_Window *window) {
    Uint32 orb_flags = ORB_WINDOW_ASYNC;
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;

    if(window->flags & SDL_WINDOW_RESIZABLE) {
        fprintf(stderr, "Setting window resizable\n");
        orb_flags |= ORB_WINDOW_RESIZABLE;
    }
    /*if(window->flags & SDL_WINDOW_SHOWN) {
        fprintf(stderr, "Setting window in foreground\n");
        orb_flags |= ORB_WINDOW_FRONT;
    }
    if(window->flags & SDL_WINDOW_HIDDEN) {
        fprintf(stderr, "Setting window in background\n");
        orb_flags |= ORB_WINDOW_BACK;
    }*/

    return init_window(window, video_data, orb_flags);
}

int ORBITAL_CreateWindowFrom(_THIS, SDL_Window * window, const void *data) {
    SDL_VideoData* video_data = (SDL_VideoData*)data;
    Uint32 width = 0;
    Uint32 height = 0;
    Sint32 x = 0;
    Sint32 y = 0;

    assert(video_data != NULL);

    width = orb_window_width(video_data->orb_window);
    height = orb_window_height(video_data->orb_window);
    x = orb_window_x(video_data->orb_window);
    y = orb_window_y(video_data->orb_window);

    fprintf(stderr, "Creating window from source at %d,%d with size %dx%d\n", x, y, width, height);

    // FIXME: add flags and title setter/getter to liborbital?
    window->x = x;
    window->y = y;
    window->w = width;
    window->h = height;

    return ORBITAL_CreateWindow(_this, window);
}

void ORBITAL_SetWindowTitle(_THIS, SDL_Window * window) {
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;
    const char* title = window->title != NULL ? window->title : DEFAULT_ORB_WINDOW_TITLE;
    assert(video_data != NULL);
    orb_window_set_title(video_data->orb_window, title);
}

void ORBITAL_SetWindowIcon(_THIS, SDL_Window* window, SDL_Surface* icon) {
    // FIXME: Icons not supported by Orbital
}

void ORBITAL_SetWindowPosition(_THIS, SDL_Window* window) {
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;
    assert(video_data != NULL);
    orb_window_set_pos(video_data->orb_window, window->x, window->y);
}

void ORBITAL_SetWindowSize(_THIS, SDL_Window * window) {
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;
    assert(video_data != NULL);
    orb_window_set_size(video_data->orb_window, window->w, window->h);
}

void ORBITAL_SetWindowBordered(_THIS, SDL_Window* window, SDL_bool bordered) {
    // FIXME: Border not supported by Orbital
}

void ORBITAL_SetWindowResizable(_THIS, SDL_Window* window, SDL_bool resizable) {
}

void ORBITAL_ShowWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_HideWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_RaiseWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_MaximizeWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_MinimizeWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_RestoreWindow(_THIS, SDL_Window* window) {
}

void ORBITAL_SetWindowFullscreen(_THIS, SDL_Window* window, SDL_VideoDisplay* display, SDL_bool fullscreen) {
}

int ORBITAL_SetWindowGammaRamp(_THIS, SDL_Window* window, const Uint16* ramp) {
    return -1;
}

int ORBITAL_GetWindowGammaRamp(_THIS, SDL_Window* window, Uint16* ramp) {
    return -1;
}

void ORBITAL_SetWindowGrab(_THIS, SDL_Window* window, SDL_bool grabbed) {
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;
    assert(video_data != NULL);
    orb_window_set_mouse_grab(video_data->orb_window, grabbed == SDL_TRUE);
}

void ORBITAL_DestroyWindow(_THIS, SDL_Window* window) {
    SDL_VideoData *video_data = (SDL_VideoData *)_this->driverdata;
    assert(video_data != NULL);
    if (video_data->orb_window != NULL) {
        orb_window_destroy(video_data->orb_window);
        video_data->orb_window = NULL;
    }
}

SDL_bool ORBITAL_GetWindowWMInfo(_THIS, SDL_Window* window, struct SDL_SysWMinfo *info) {
    // FIXME: What is the point of this? What information should be included?
    return SDL_FALSE;
}

#ifdef __cplusplus
}
#endif
