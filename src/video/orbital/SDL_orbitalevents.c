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
#include <SDL_config.h>

#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include <orbital.h>
#include "SDL_orbitalopengl.h"
#include "SDL_orbitalvideo.h"
#include "SDL_orbitalevents.h"

#include <assert.h>
#include <unistd.h>

/* Static variables so only changes are reported */
static bool last_button_left = false;
static bool last_button_middle = false;
static bool last_button_right = false;

static SDL_Keycode keymap[256];

void ORBITAL_PumpEvents(_THIS)
{
    SDL_Keysym keysym;
    void* event_iter;
    OrbEventOption oeo;
    int resize_width = -1, resize_height = -1;
    SDL_Mouse *mouse = SDL_GetMouse();
    SDL_VideoData* video_data = _this->driverdata;
    assert(video_data != NULL);

    event_iter = orb_window_events(video_data->orb_window);

    oeo = orb_events_next(event_iter);
    while (oeo.tag != OrbEventOption_None) {
        switch (oeo.tag) {
            case OrbEventOption_Key:
                keysym.sym = keymap[oeo.key.scancode];
                keysym.scancode = SDL_GetScancodeFromKey(keysym.sym);
                keysym.mod = KMOD_NONE;
                SDL_SendKeyboardKey(oeo.key.pressed ? SDL_PRESSED : SDL_RELEASED, keysym.scancode);
                break;
            case OrbEventOption_Mouse:
                SDL_SendMouseMotion(mouse->focus, mouse->mouseID, SDL_FALSE, oeo.mouse.x, oeo.mouse.y);
                break;
            case OrbEventOption_MouseRelative:
                SDL_SendMouseMotion(mouse->focus, mouse->mouseID, SDL_TRUE, oeo.mouse_relative.dx, oeo.mouse_relative.dy);
                break;
            case OrbEventOption_Button:
                if (oeo.button.left ^ last_button_left)
                    SDL_SendMouseButton(mouse->focus, mouse->mouseID, oeo.button.left ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_LEFT);
                if (oeo.button.middle ^ last_button_middle)
                    SDL_SendMouseButton(mouse->focus, mouse->mouseID, oeo.button.middle ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_MIDDLE);
                if (oeo.button.right ^ last_button_right)
                    SDL_SendMouseButton(mouse->focus, mouse->mouseID, oeo.button.right ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_RIGHT);

                last_button_left = oeo.button.left;
                last_button_middle = oeo.button.middle;
                last_button_right = oeo.button.right;
                break;
            case OrbEventOption_Scroll:
                SDL_SendMouseWheel(mouse->focus, mouse->mouseID, oeo.scroll.x, oeo.scroll.y, SDL_MOUSEWHEEL_NORMAL);
                break;
            case OrbEventOption_Quit:
                SDL_Quit();
                break;
            case OrbEventOption_Focus:
                //SDL_PrivateAppActive(oeo.focus.focused, SDL_APPMOUSEFOCUS);
                break;
            case OrbEventOption_Move:
                SDL_SendWindowEvent(video_data->window, SDL_WINDOWEVENT_MOVED, oeo.move.x, oeo.move.y);
                break;
            case OrbEventOption_Resize:
                // Combine expensive resizes
                resize_width = oeo.resize.width;
                resize_height = oeo.resize.height;
                break;
            case OrbEventOption_Screen:
                // oeo.screen.width, oeo.screen.height
                break;
            case OrbEventOption_Unknown:
                // oeo.unknown.code, oeo.unknown.a, oeo.unknown.b
                break;
            default:
                break;
        }

        oeo = orb_events_next(event_iter);
    }

    orb_events_destroy(event_iter);

    // Perform the latest resize
    if (resize_width >= 0 && resize_height >= 0) {
        // Set size of mesa context
        ORBITAL_GL_SetSize(_this, resize_width, resize_height);
        // Send resize event
        SDL_SendWindowEvent(video_data->window, SDL_WINDOWEVENT_RESIZED, resize_width, resize_height);
    }
}

void ORBITAL_InitOSKeymap()
{
    for (int i = 0; i < SDL_arraysize(keymap); ++i )
        keymap[i] = SDLK_UNKNOWN;

    keymap[ORB_KEY_ESC] = SDLK_ESCAPE;
    keymap[ORB_KEY_1] = SDLK_1;
    keymap[ORB_KEY_2] = SDLK_2;
    keymap[ORB_KEY_3] = SDLK_3;
    keymap[ORB_KEY_4] = SDLK_4;
    keymap[ORB_KEY_5] = SDLK_5;
    keymap[ORB_KEY_6] = SDLK_6;
    keymap[ORB_KEY_7] = SDLK_7;
    keymap[ORB_KEY_8] = SDLK_8;
    keymap[ORB_KEY_9] = SDLK_9;
    keymap[ORB_KEY_0] = SDLK_0;
    keymap[ORB_KEY_MINUS] = SDLK_MINUS;
    keymap[ORB_KEY_EQUALS] = SDLK_EQUALS;
    keymap[ORB_KEY_BKSP] = SDLK_BACKSPACE;
    keymap[ORB_KEY_TAB] = SDLK_TAB;
    keymap[ORB_KEY_Q] = SDLK_q;
    keymap[ORB_KEY_W] = SDLK_w;
    keymap[ORB_KEY_E] = SDLK_e;
    keymap[ORB_KEY_R] = SDLK_r;
    keymap[ORB_KEY_T] = SDLK_t;
    keymap[ORB_KEY_Y] = SDLK_y;
    keymap[ORB_KEY_U] = SDLK_u;
    keymap[ORB_KEY_I] = SDLK_i;
    keymap[ORB_KEY_O] = SDLK_o;
    keymap[ORB_KEY_P] = SDLK_p;
    keymap[ORB_KEY_BRACE_OPEN] = SDLK_LEFTBRACKET;
    keymap[ORB_KEY_BRACE_CLOSE] = SDLK_RIGHTBRACKET;
    keymap[ORB_KEY_ENTER] = SDLK_RETURN;
    keymap[ORB_KEY_CTRL] = SDLK_LCTRL;
    keymap[ORB_KEY_A] = SDLK_a;
    keymap[ORB_KEY_S] = SDLK_s;
    keymap[ORB_KEY_D] = SDLK_d;
    keymap[ORB_KEY_F] = SDLK_f;
    keymap[ORB_KEY_G] = SDLK_g;
    keymap[ORB_KEY_H] = SDLK_h;
    keymap[ORB_KEY_J] = SDLK_j;
    keymap[ORB_KEY_K] = SDLK_k;
    keymap[ORB_KEY_L] = SDLK_l;
    keymap[ORB_KEY_SEMICOLON] = SDLK_SEMICOLON;
    keymap[ORB_KEY_QUOTE] = SDLK_QUOTE;
    keymap[ORB_KEY_TICK] = SDLK_BACKQUOTE;
    keymap[ORB_KEY_LEFT_SHIFT] = SDLK_LSHIFT;
    keymap[ORB_KEY_RIGHT_SHIFT] = SDLK_RSHIFT;
    keymap[ORB_KEY_BACKSLASH] = SDLK_BACKSLASH;
    keymap[ORB_KEY_Z] = SDLK_z;
    keymap[ORB_KEY_X] = SDLK_x;
    keymap[ORB_KEY_C] = SDLK_c;
    keymap[ORB_KEY_V] = SDLK_v;
    keymap[ORB_KEY_B] = SDLK_b;
    keymap[ORB_KEY_N] = SDLK_n;
    keymap[ORB_KEY_M] = SDLK_m;
    keymap[ORB_KEY_COMMA] = SDLK_COMMA;
    keymap[ORB_KEY_PERIOD] = SDLK_PERIOD;
    keymap[ORB_KEY_SLASH] = SDLK_SLASH;
    keymap[ORB_KEY_ALT] = SDLK_LALT;
    keymap[ORB_KEY_SPACE] = SDLK_SPACE;
    keymap[ORB_KEY_CAPS] = SDLK_CAPSLOCK;
    keymap[ORB_KEY_F1] = SDLK_F1;
    keymap[ORB_KEY_F2] = SDLK_F2;
    keymap[ORB_KEY_F3] = SDLK_F3;
    keymap[ORB_KEY_F4] = SDLK_F4;
    keymap[ORB_KEY_F5] = SDLK_F5;
    keymap[ORB_KEY_F6] = SDLK_F6;
    keymap[ORB_KEY_F7] = SDLK_F7;
    keymap[ORB_KEY_F8] = SDLK_F8;
    keymap[ORB_KEY_F9] = SDLK_F9;
    keymap[ORB_KEY_F10] = SDLK_F10;
    keymap[ORB_KEY_F11] = SDLK_F11;
    keymap[ORB_KEY_F12] = SDLK_F12;
    keymap[ORB_KEY_HOME] = SDLK_HOME;
    keymap[ORB_KEY_UP] = SDLK_UP;
    keymap[ORB_KEY_PGUP] = SDLK_PAGEUP;
    keymap[ORB_KEY_LEFT] = SDLK_LEFT;
    keymap[ORB_KEY_RIGHT] = SDLK_RIGHT;
    keymap[ORB_KEY_END] = SDLK_END;
    keymap[ORB_KEY_DOWN] = SDLK_DOWN;
    keymap[ORB_KEY_PGDN] = SDLK_PAGEDOWN;
    keymap[ORB_KEY_INSERT] = SDLK_INSERT;
    keymap[ORB_KEY_DEL] = SDLK_DELETE;

    fprintf(stderr, "SDL orbital keymap initialized\n");
}
