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
#include "SDL_orbitalvideo.h"
#include "SDL_orbitalevents.h"

#include <unistd.h>

/* Static variables so only changes are reported */
static bool last_button_left = false;
static bool last_button_middle = false;
static bool last_button_right = false;

#define KEYMAP_SIZE 128

static SDL_Keycode keymap[KEYMAP_SIZE];
static int8_t keystate[KEYMAP_SIZE];

void ORBITAL_PumpEvents(_THIS)
{
    SDL_Keysym keysym;
    SDL_Mouse *mouse = SDL_GetMouse();
    SDL_VideoData* video_data = _this->driverdata;

    void* event_iter = orb_window_events(video_data->orb_window);
    OrbEventOption oeo = orb_events_next(event_iter);

    while (oeo.tag != OrbEventOption_None) {
        switch (oeo.tag) {
            case OrbEventOption_Key:
                keysym.scancode = oeo.key.scancode;
                keysym.sym = keymap[oeo.key.scancode];
                keysym.mod = KMOD_NONE;

                SDL_SendKeyboardKey(oeo.key.pressed ? SDL_PRESSED : SDL_RELEASED, keysym.scancode);
                break;
            case OrbEventOption_Mouse:
                SDL_SendMouseMotion(mouse->focus, mouse->mouseID, SDL_FALSE, oeo.mouse.x, oeo.mouse.y);
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
                // oeo.move.x, oeo.move.y
                break;
            case OrbEventOption_Resize:
                //SDL_PrivateResize(oeo.resize.width, oeo.resize.height);
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
}

void ORBITAL_InitOSKeymap()
{
    for (int i = 0; i < SDL_arraysize(keymap); ++i )
        keymap[i] = SDLK_UNKNOWN;

    keymap[ORB_KEY_ESC] = SDL_GetScancodeFromKey(SDLK_ESCAPE);
    keymap[ORB_KEY_1] = SDL_GetScancodeFromKey(SDLK_1);
    keymap[ORB_KEY_2] = SDL_GetScancodeFromKey(SDLK_2);
    keymap[ORB_KEY_3] = SDL_GetScancodeFromKey(SDLK_3);
    keymap[ORB_KEY_4] = SDL_GetScancodeFromKey(SDLK_4);
    keymap[ORB_KEY_5] = SDL_GetScancodeFromKey(SDLK_5);
    keymap[ORB_KEY_6] = SDL_GetScancodeFromKey(SDLK_6);
    keymap[ORB_KEY_7] = SDL_GetScancodeFromKey(SDLK_7);
    keymap[ORB_KEY_8] = SDL_GetScancodeFromKey(SDLK_8);
    keymap[ORB_KEY_9] = SDL_GetScancodeFromKey(SDLK_9);
    keymap[ORB_KEY_0] = SDL_GetScancodeFromKey(SDLK_0);
    keymap[ORB_KEY_MINUS] = SDL_GetScancodeFromKey(SDLK_MINUS);
    keymap[ORB_KEY_EQUALS] = SDL_GetScancodeFromKey(SDLK_EQUALS);
    keymap[ORB_KEY_BKSP] = SDL_GetScancodeFromKey(SDLK_BACKSPACE);
    keymap[ORB_KEY_TAB] = SDL_GetScancodeFromKey(SDLK_TAB);
    keymap[ORB_KEY_Q] = SDL_GetScancodeFromKey(SDLK_q);
    keymap[ORB_KEY_W] = SDL_GetScancodeFromKey(SDLK_w);
    keymap[ORB_KEY_E] = SDL_GetScancodeFromKey(SDLK_e);
    keymap[ORB_KEY_R] = SDL_GetScancodeFromKey(SDLK_r);
    keymap[ORB_KEY_T] = SDL_GetScancodeFromKey(SDLK_t);
    keymap[ORB_KEY_Y] = SDL_GetScancodeFromKey(SDLK_y);
    keymap[ORB_KEY_U] = SDL_GetScancodeFromKey(SDLK_u);
    keymap[ORB_KEY_I] = SDL_GetScancodeFromKey(SDLK_i);
    keymap[ORB_KEY_O] = SDL_GetScancodeFromKey(SDLK_o);
    keymap[ORB_KEY_P] = SDL_GetScancodeFromKey(SDLK_p);
    keymap[ORB_KEY_BRACE_OPEN] = SDL_GetScancodeFromKey(SDLK_LEFTBRACKET);
    keymap[ORB_KEY_BRACE_CLOSE] = SDL_GetScancodeFromKey(SDLK_RIGHTBRACKET);
    keymap[ORB_KEY_ENTER] = SDL_GetScancodeFromKey(SDLK_RETURN);
    keymap[ORB_KEY_CTRL] = SDL_GetScancodeFromKey(SDLK_LCTRL);
    keymap[ORB_KEY_A] = SDL_GetScancodeFromKey(SDLK_a);
    keymap[ORB_KEY_S] = SDL_GetScancodeFromKey(SDLK_s);
    keymap[ORB_KEY_D] = SDL_GetScancodeFromKey(SDLK_d);
    keymap[ORB_KEY_F] = SDL_GetScancodeFromKey(SDLK_f);
    keymap[ORB_KEY_G] = SDL_GetScancodeFromKey(SDLK_g);
    keymap[ORB_KEY_H] = SDL_GetScancodeFromKey(SDLK_h);
    keymap[ORB_KEY_J] = SDL_GetScancodeFromKey(SDLK_j);
    keymap[ORB_KEY_K] = SDL_GetScancodeFromKey(SDLK_k);
    keymap[ORB_KEY_L] = SDL_GetScancodeFromKey(SDLK_l);
    keymap[ORB_KEY_SEMICOLON] = SDL_GetScancodeFromKey(SDLK_SEMICOLON);
    keymap[ORB_KEY_QUOTE] = SDL_GetScancodeFromKey(SDLK_QUOTE);
    keymap[ORB_KEY_TICK] = SDL_GetScancodeFromKey(SDLK_BACKQUOTE);
    keymap[ORB_KEY_LEFT_SHIFT] = SDL_GetScancodeFromKey(SDLK_LSHIFT);
    keymap[ORB_KEY_RIGHT_SHIFT] = SDL_GetScancodeFromKey(SDLK_RSHIFT);
    keymap[ORB_KEY_BACKSLASH] = SDL_GetScancodeFromKey(SDLK_BACKSLASH);
    keymap[ORB_KEY_Z] = SDL_GetScancodeFromKey(SDLK_z);
    keymap[ORB_KEY_X] = SDL_GetScancodeFromKey(SDLK_x);
    keymap[ORB_KEY_C] = SDL_GetScancodeFromKey(SDLK_c);
    keymap[ORB_KEY_V] = SDL_GetScancodeFromKey(SDLK_v);
    keymap[ORB_KEY_B] = SDL_GetScancodeFromKey(SDLK_b);
    keymap[ORB_KEY_N] = SDL_GetScancodeFromKey(SDLK_n);
    keymap[ORB_KEY_M] = SDL_GetScancodeFromKey(SDLK_m);
    keymap[ORB_KEY_COMMA] = SDL_GetScancodeFromKey(SDLK_COMMA);
    keymap[ORB_KEY_PERIOD] = SDL_GetScancodeFromKey(SDLK_PERIOD);
    keymap[ORB_KEY_SLASH] = SDL_GetScancodeFromKey(SDLK_SLASH);
    keymap[ORB_KEY_ALT] = SDL_GetScancodeFromKey(SDLK_LALT);
    keymap[ORB_KEY_SPACE] = SDL_GetScancodeFromKey(SDLK_SPACE);
    keymap[ORB_KEY_CAPS] = SDL_GetScancodeFromKey(SDLK_CAPSLOCK);
    keymap[ORB_KEY_F1] = SDL_GetScancodeFromKey(SDLK_F1);
    keymap[ORB_KEY_F2] = SDL_GetScancodeFromKey(SDLK_F2);
    keymap[ORB_KEY_F3] = SDL_GetScancodeFromKey(SDLK_F3);
    keymap[ORB_KEY_F4] = SDL_GetScancodeFromKey(SDLK_F4);
    keymap[ORB_KEY_F5] = SDL_GetScancodeFromKey(SDLK_F5);
    keymap[ORB_KEY_F6] = SDL_GetScancodeFromKey(SDLK_F6);
    keymap[ORB_KEY_F7] = SDL_GetScancodeFromKey(SDLK_F7);
    keymap[ORB_KEY_F8] = SDL_GetScancodeFromKey(SDLK_F8);
    keymap[ORB_KEY_F9] = SDL_GetScancodeFromKey(SDLK_F9);
    keymap[ORB_KEY_F10] = SDL_GetScancodeFromKey(SDLK_F10);
    keymap[ORB_KEY_F11] = SDL_GetScancodeFromKey(SDLK_F11);
    keymap[ORB_KEY_F12] = SDL_GetScancodeFromKey(SDLK_F12);
    keymap[ORB_KEY_HOME] = SDL_GetScancodeFromKey(SDLK_HOME);
    keymap[ORB_KEY_UP] = SDL_GetScancodeFromKey(SDLK_UP);
    keymap[ORB_KEY_PGUP] = SDL_GetScancodeFromKey(SDLK_PAGEUP);
    keymap[ORB_KEY_LEFT] = SDL_GetScancodeFromKey(SDLK_LEFT);
    keymap[ORB_KEY_RIGHT] = SDL_GetScancodeFromKey(SDLK_RIGHT);
    keymap[ORB_KEY_END] = SDL_GetScancodeFromKey(SDLK_END);
    keymap[ORB_KEY_DOWN] = SDL_GetScancodeFromKey(SDLK_DOWN);
    keymap[ORB_KEY_PGDN] = SDL_GetScancodeFromKey(SDLK_PAGEDOWN);
    keymap[ORB_KEY_INSERT] = SDL_GetScancodeFromKey(SDLK_INSERT);
    keymap[ORB_KEY_DEL] = SDL_GetScancodeFromKey(SDLK_DELETE);

    fprintf(stderr, "SDL orbital keymap initialized\n");
}

SDL_Scancode ORBITAL_GetScancodeFromBeKey(int32_t key) {
    if(key > 0 && key < (int32_t)SDL_TABLESIZE(keymap)) {
        return keymap[key];
    } else {
        return SDL_SCANCODE_UNKNOWN;
    }
}

int8_t ORBITAL_GetKeyState(int32_t key) {
    if(key > 0 && key < KEYMAP_SIZE) {
        return keystate[key];
    } else {
        return SDL_RELEASED;
    }
}

void ORBITAL_SetKeyState(int32_t key, int8_t state) {
    if(key > 0 && key < KEYMAP_SIZE) {
        keystate[key] = state;
    }
}

/* end of SDL_orbitalevents.c ... */
