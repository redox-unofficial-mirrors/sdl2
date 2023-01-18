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

#include "SDL_orbitalopengl.h"
#include "SDL_orbitalvideo.h"

#include <GL/osmesa.h>
#include <orbital.h>
#include <assert.h>

/* Passing a NULL path means load pointers from the application */
int ORBITAL_GL_LoadLibrary(_THIS, const char *path)
{
    return 0;
}

void* ORBITAL_GL_GetProcAddress(_THIS, const char *proc)
{
    return OSMesaGetProcAddress(proc);
}

int ORBITAL_GL_SwapWindow(_THIS, SDL_Window* window) {
    SDL_VideoData* video_data = _this->driverdata;
    Uint32 width = orb_window_width(video_data->orb_window);
    Uint32 height = orb_window_height(video_data->orb_window);
    Uint32* frame_data = orb_window_data(video_data->orb_window);
    Uint32* image_data = video_data->orb_buffer;

    // It is currently required to flush for video data to be updated
    glFlush();

    for(int i = 0; i < width * height; ++i) {
        frame_data[i] = image_data[i] | 0xFF000000;
    }

    orb_window_sync(video_data->orb_window);
    return 0;
}

int ORBITAL_GL_MakeCurrent(_THIS, SDL_Window* window, SDL_GLContext ctx) {
    SDL_VideoData* video_data = _this->driverdata;
    OSMesaContext context = ctx;
    void* buffer = NULL;
    Uint32 width = 0;
    Uint32 height = 0;

    if (ctx == NULL)
        return -1;

    assert(video_data != NULL);

    width = orb_window_width(video_data->orb_window);
    height = orb_window_height(video_data->orb_window);
    buffer = malloc(width * height * 4);
    if (buffer == NULL) {
        fprintf(stderr, "Frame buffer allocation failed\n");
        OSMesaDestroyContext(context);
        SDL_OutOfMemory();
        return -1;
    }

    //fprintf(stderr, "Frame buffer allocated\n");

    if (!OSMesaMakeCurrent(context, buffer, GL_UNSIGNED_BYTE, width, height)) {
        fprintf(stderr, "OSMesaMakeCurrent failed\n");
        SDL_free(buffer);
        OSMesaDestroyContext(context);
        return -1;
    }

    //fprintf(stderr, "OSMesaMakeCurrent done\n");

    OSMesaPixelStore(OSMESA_Y_UP, 0);
    OSMesaColorClamp(GL_TRUE);

    video_data->orb_buffer = buffer;
    video_data->mesa_context = context;

    fprintf(stderr, "SDL orbital OSMesa buffer binded to context\n");

    return 0;
}


SDL_GLContext ORBITAL_GL_CreateContext(_THIS, SDL_Window* window) {
    // Fixme: verify _this->gl_config.double_buffer
    OSMesaContext ctx = OSMesaCreateContextExt(OSMESA_BGRA, _this->gl_config.depth_size, _this->gl_config.stencil_size, 0, NULL);
    if (ctx == NULL) {
        fprintf(stderr, "OSMesaCreateContextExt failed\n");
        return NULL;
    }

    fprintf(stderr, "SDL orbital OSMesa context created successfully\n");

    return ORBITAL_GL_MakeCurrent(_this, window, ctx) >= 0 ? ctx : NULL;
}

void ORBITAL_GL_DeleteContext(_THIS, SDL_GLContext context) {
    OSMesaDestroyContext(context);
}


int ORBITAL_GL_SetSwapInterval(_THIS, int interval) {
    // TODO: Implement this, if necessary?
    return 0;
}

int ORBITAL_GL_GetSwapInterval(_THIS) {
    // TODO: Implement this, if necessary?
    return 0;
}


void ORBITAL_GL_UnloadLibrary(_THIS) {
    // TODO: Implement this, if necessary?
}
