/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

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

#ifdef SDL_JOYSTICK_REDOX

/* This is the Redox OS implementation of the SDL joystick API */
#include <orbital.h>

#include "../SDL_sysjoystick.h"
#include "SDL_events.h"

static int REDOX_JoystickInit(void)
{
    return 0;
}

static const char *REDOX_JoystickGetDeviceName(int device_index)
{
    return "Redox OS Controller";
}

static int REDOX_JoystickGetCount(void)
{
    //TODO: get actual count or dynamically add and remove
    return 1;
}

static SDL_JoystickGUID REDOX_JoystickGetDeviceGUID(int device_index)
{
    SDL_JoystickGUID guid = SDL_CreateJoystickGUIDForName("Redox OS");
    return guid;
}

static SDL_JoystickID REDOX_JoystickGetDeviceInstanceID(int device_index)
{
    return device_index;
}

static int REDOX_JoystickOpen(SDL_Joystick *joystick, int device_index)
{
    joystick->nbuttons = 15;
    joystick->naxes = 6;
    joystick->nhats = 0;
    joystick->instance_id = device_index;

    return 0;
}

static int REDOX_JoystickSetSensorsEnabled(SDL_Joystick *joystick, SDL_bool enabled)
{
    return SDL_Unsupported();
}

static void REDOX_JoystickUpdate(SDL_Joystick *joystick)
{
    //printf("UPDATE JOYSTICK %d\n", joystick->instance_id);
}

static void REDOX_JoystickClose(SDL_Joystick *joystick)
{
}

static void REDOX_JoystickQuit(void)
{
}

static SDL_bool REDOX_JoystickGetGamepadMapping(int device_index, SDL_GamepadMapping *out)
{
    /* There is only one possible mapping. */
    *out = (SDL_GamepadMapping){
        /* Axes */
        .leftx = { EMappingKind_Axis, 0 },
        .lefty = { EMappingKind_Axis, 1 },
        .lefttrigger = { EMappingKind_Axis, 2 },
        .rightx = { EMappingKind_Axis, 3 },
        .righty = { EMappingKind_Axis, 4 },
        .righttrigger = { EMappingKind_Axis, 5 },
        /* Buttons  */
        .a = { EMappingKind_Button, 0 },
        .b = { EMappingKind_Button, 1 },
        .x = { EMappingKind_Button, 2 },
        .y = { EMappingKind_Button, 3 },
        .leftshoulder = { EMappingKind_Button, 4 },
        .rightshoulder = { EMappingKind_Button, 5 },
        .back = { EMappingKind_Button, 6 },
        .start = { EMappingKind_Button, 7 },
        .leftstick = { EMappingKind_Button, 8 },
        .rightstick = { EMappingKind_Button, 9 },
        .guide = { EMappingKind_Button, 10 },
        .dpup = { EMappingKind_Button, 11 },
        .dpdown = { EMappingKind_Button, 12 },
        .dpleft = { EMappingKind_Button, 13 },
        .dpright = { EMappingKind_Button, 14 },
        /* Unmapped  */
        .misc1 = { EMappingKind_None, 255 },
        .paddle1 = { EMappingKind_None, 255 },
        .paddle2 = { EMappingKind_None, 255 },
        .paddle3 = { EMappingKind_None, 255 },
        .paddle4 = { EMappingKind_None, 255 },
    };
    return SDL_TRUE;
}

static void REDOX_JoystickDetect(void)
{
}

static const char *REDOX_JoystickGetDevicePath(int device_index)
{
    return NULL;
}

static int REDOX_JoystickGetDeviceSteamVirtualGamepadSlot(int device_index)
{
    return -1;
}

static int REDOX_JoystickGetDevicePlayerIndex(int device_index)
{
    return -1;
}

static void REDOX_JoystickSetDevicePlayerIndex(int device_index, int player_index)
{
}

static Uint32 REDOX_JoystickGetCapabilities(SDL_Joystick *joystick)
{
    return 0;
}

static int REDOX_JoystickRumble(SDL_Joystick *joystick, Uint16 low_frequency_rumble, Uint16 high_frequency_rumble)
{
    return SDL_Unsupported();
}

static int REDOX_JoystickRumbleTriggers(SDL_Joystick *joystick, Uint16 left_rumble, Uint16 right_rumble)
{
    return SDL_Unsupported();
}

static int REDOX_JoystickSetLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue)
{
    return SDL_Unsupported();
}

static int REDOX_JoystickSendEffect(SDL_Joystick *joystick, const void *data, int size)
{
    return SDL_Unsupported();
}

SDL_JoystickDriver SDL_REDOX_JoystickDriver = {
    .Init = REDOX_JoystickInit,
    .GetCount = REDOX_JoystickGetCount,
    .Detect = REDOX_JoystickDetect,
    .GetDeviceName = REDOX_JoystickGetDeviceName,
    .GetDevicePath = REDOX_JoystickGetDevicePath,
    .GetDeviceSteamVirtualGamepadSlot = REDOX_JoystickGetDeviceSteamVirtualGamepadSlot,
    .GetDevicePlayerIndex = REDOX_JoystickGetDevicePlayerIndex,
    .SetDevicePlayerIndex = REDOX_JoystickSetDevicePlayerIndex,
    .GetDeviceGUID = REDOX_JoystickGetDeviceGUID,
    .GetDeviceInstanceID = REDOX_JoystickGetDeviceInstanceID,
    .Open = REDOX_JoystickOpen,
    .Rumble = REDOX_JoystickRumble,
    .RumbleTriggers = REDOX_JoystickRumbleTriggers,
    .GetCapabilities = REDOX_JoystickGetCapabilities,
    .SetLED = REDOX_JoystickSetLED,
    .SendEffect = REDOX_JoystickSendEffect,
    .SetSensorsEnabled = REDOX_JoystickSetSensorsEnabled,
    .Update = REDOX_JoystickUpdate,
    .Close = REDOX_JoystickClose,
    .Quit = REDOX_JoystickQuit,
    .GetGamepadMapping = REDOX_JoystickGetGamepadMapping
};

#endif /* SDL_JOYSTICK_REDOX */

/* vi: set sts=4 ts=4 sw=4 expandtab: */
