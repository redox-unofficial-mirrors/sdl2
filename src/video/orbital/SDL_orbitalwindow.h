#ifndef SDL_ORBITALWINDOW_H
#define SDL_ORBITALWINDOW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../SDL_sysvideo.h"

extern int ORBITAL_CreateWindow(_THIS, SDL_Window *window);
extern int ORBITAL_CreateWindowFrom(_THIS, SDL_Window * window, const void *data);
extern void ORBITAL_SetWindowTitle(_THIS, SDL_Window * window);
extern void ORBITAL_SetWindowIcon(_THIS, SDL_Window * window, SDL_Surface * icon);
extern void ORBITAL_SetWindowPosition(_THIS, SDL_Window * window);
extern void ORBITAL_SetWindowSize(_THIS, SDL_Window * window);
extern void ORBITAL_ShowWindow(_THIS, SDL_Window * window);
extern void ORBITAL_HideWindow(_THIS, SDL_Window * window);
extern void ORBITAL_RaiseWindow(_THIS, SDL_Window * window);
extern void ORBITAL_MaximizeWindow(_THIS, SDL_Window * window);
extern void ORBITAL_MinimizeWindow(_THIS, SDL_Window * window);
extern void ORBITAL_RestoreWindow(_THIS, SDL_Window * window);
extern void ORBITAL_SetWindowBordered(_THIS, SDL_Window * window, SDL_bool bordered);
extern void ORBITAL_SetWindowResizable(_THIS, SDL_Window * window, SDL_bool resizable);
extern void ORBITAL_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen);
extern int ORBITAL_SetWindowGammaRamp(_THIS, SDL_Window * window, const Uint16 * ramp);
extern int ORBITAL_GetWindowGammaRamp(_THIS, SDL_Window * window, Uint16 * ramp);
extern void ORBITAL_SetWindowGrab(_THIS, SDL_Window * window, SDL_bool grabbed);
extern void ORBITAL_DestroyWindow(_THIS, SDL_Window * window);
extern SDL_bool ORBITAL_GetWindowWMInfo(_THIS, SDL_Window * window,
                                    struct SDL_SysWMinfo *info);


#ifdef __cplusplus
}
#endif

#endif