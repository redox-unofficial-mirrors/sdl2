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
#ifndef SDL_ORBITALEVENTS_H
#define SDL_ORBITALEVENTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <SDL_scancode.h>

extern void ORBITAL_PumpEvents(_THIS);
extern void ORBITAL_InitOSKeymap();
extern SDL_Scancode ORBITAL_GetScancodeFromOrbitalKey(Sint32 key);
extern int8_t ORBITAL_GetKeyState(Sint32 key);
extern void ORBITAL_SetKeyState(Sint32 bkey, Sint8 state);

#ifdef __cplusplus
}
#endif

#endif // SDL_ORBITALEVENTS_H
