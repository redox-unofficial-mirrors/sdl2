/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/* Orbital SDL video driver implementation
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 *  Repurposed to ORBITAL by Jeremy Soller.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include <orbital.h>
#include "SDL_orbitalvideo.h"
#include "SDL_orbitalevents.h"
#include "SDL_orbitalwindow.h"
#include "SDL_orbitalframebuffer.h"
#include "SDL_orbitalopengl.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define ORBITALVID_DRIVER_NAME "orbital"

/* Initialization/Query functions */
static int ORBITAL_VideoInit(_THIS);
static void ORBITAL_VideoQuit(_THIS);

/* ORBITAL driver bootstrap functions */

static int ORBITAL_Available(void)
{
	return 1;
}

static void ORBITAL_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_VideoData* video_data = device->driverdata;
    if (video_data->orb_window != NULL) {
	    ORBITAL_DestroyWindow(device, video_data->orb_window);
    }
    if (video_data->mesa_context != NULL) {
	    ORBITAL_GL_DeleteContext(device, video_data->mesa_context);
    }
    if (video_data->orb_buffer != NULL) {
        SDL_free(video_data->orb_buffer);
    }
	
    SDL_free(device->driverdata);
	SDL_free(device);

    fprintf(stderr, "SDL orbital device disposed\n");
}

static SDL_VideoDevice* ORBITAL_CreateDevice(int devindex)
{
	SDL_VideoDevice* device = NULL;
    SDL_VideoData* video_data = NULL;

    //fprintf(stderr, "Creating video device\n");
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
	if (device == NULL) {
		SDL_OutOfMemory();
		return NULL;
	}

    //fprintf(stderr, "Creating orbital video data\n");
    video_data = (SDL_VideoData *) SDL_calloc(1, sizeof(SDL_VideoData));
	if (video_data == NULL) {
        SDL_free(device);
		SDL_OutOfMemory();
		return NULL;
	}

	device->driverdata = video_data;
    //fprintf(stderr, "device->driverdata = %p\n", device->driverdata);

	/* Set the function pointers */
	device->VideoInit = ORBITAL_VideoInit;
	device->VideoQuit = ORBITAL_VideoQuit;

	device->PumpEvents = ORBITAL_PumpEvents;

	device->CreateSDLWindow = ORBITAL_CreateWindow;
    device->CreateSDLWindowFrom = ORBITAL_CreateWindowFrom;
    device->SetWindowTitle = ORBITAL_SetWindowTitle;
    device->SetWindowIcon = ORBITAL_SetWindowIcon;
    device->SetWindowPosition = ORBITAL_SetWindowPosition;
    device->SetWindowSize = ORBITAL_SetWindowSize;
    device->ShowWindow = ORBITAL_ShowWindow;
    device->HideWindow = ORBITAL_HideWindow;
    device->RaiseWindow = ORBITAL_RaiseWindow;
    device->MaximizeWindow = ORBITAL_MaximizeWindow;
    device->MinimizeWindow = ORBITAL_MinimizeWindow;
    device->RestoreWindow = ORBITAL_RestoreWindow;
    device->SetWindowBordered = ORBITAL_SetWindowBordered;
    device->SetWindowResizable = ORBITAL_SetWindowResizable;
    device->SetWindowFullscreen = ORBITAL_SetWindowFullscreen;
    device->SetWindowGammaRamp = ORBITAL_SetWindowGammaRamp;
    device->GetWindowGammaRamp = ORBITAL_GetWindowGammaRamp;
    device->SetWindowGrab = ORBITAL_SetWindowGrab;
    device->DestroyWindow = ORBITAL_DestroyWindow;
    device->GetWindowWMInfo = ORBITAL_GetWindowWMInfo;

    // Fixme: Explicit window buffer management should be done here?
	/*device->CreateWindowFramebuffer = ORBITAL_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = ORBITAL_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = ORBITAL_DestroyWindowFramebuffer;*/

    device->GL_LoadLibrary = ORBITAL_GL_LoadLibrary;
    device->GL_GetProcAddress = ORBITAL_GL_GetProcAddress;
    device->GL_UnloadLibrary = ORBITAL_GL_UnloadLibrary;
    device->GL_CreateContext = ORBITAL_GL_CreateContext;
    device->GL_MakeCurrent = ORBITAL_GL_MakeCurrent;
    device->GL_SetSwapInterval = ORBITAL_GL_SetSwapInterval;
    device->GL_GetSwapInterval = ORBITAL_GL_GetSwapInterval;
    device->GL_SwapWindow = ORBITAL_GL_SwapWindow;
    device->GL_DeleteContext = ORBITAL_GL_DeleteContext;

	device->free = ORBITAL_DeleteDevice;

	device->num_displays = 0;

    fprintf(stderr, "SDL orbital device created successfully\n");

	return device;
}

VideoBootStrap ORBITAL_bootstrap = {
	ORBITALVID_DRIVER_NAME, "SDL orbital video driver",
	ORBITAL_Available, ORBITAL_CreateDevice
};


int ORBITAL_VideoInit(_THIS)
{
	SDL_DisplayMode desktop_mode;

	fprintf(stderr, "WARNING: You are using the SDL orbital video driver!\n");

	ORBITAL_InitOSKeymap();

	SDL_zero(desktop_mode);
	desktop_mode.w = orb_display_width();
	desktop_mode.h = orb_display_height();

	SDL_AddBasicVideoDisplay(&desktop_mode);

	fprintf(stderr, "Video display initialized successfully\n");

	// We're done!
	return 0;
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void ORBITAL_VideoQuit(_THIS)
{
	ORBITAL_DeleteDevice(_this);
}
