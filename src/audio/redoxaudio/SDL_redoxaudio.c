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

    This file written by Ryan C. Gordon (icculus@icculus.org)
*/
#include "../../SDL_internal.h"
#include "SDL_redoxaudio.h"
#include <SDL_audio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../SDL_audiodev_c.h"

// Hidden "this" pointer for the video functions
#define _THIS SDL_AudioDevice *this

// The tag name used by REDOX audio
#define REDOXAUDIO_DRIVER_NAME "redoxaudio"

// Device driver functions
static void REDOXAUDIO_DetectDevices(void);
static int REDOXAUDIO_OpenDevice(_THIS, const char *devname);
static void REDOXAUDIO_WaitDevice(_THIS);
static void REDOXAUDIO_PlayDevice(_THIS);
static Uint8 *REDOXAUDIO_GetDeviceBuf(_THIS);
static void REDOXAUDIO_CloseDevice(_THIS);
static SDL_bool REDOXAUDIO_Init(SDL_AudioDriverImpl *impl);

static void
REDOXAUDIO_DetectDevices(void)
{
}

// This function waits until it is possible to write a full sound buffer
static void REDOXAUDIO_WaitDevice(_THIS)
{
}

static void REDOXAUDIO_PlayDevice(_THIS)
{
	Uint32 written = write(this->hidden->output,
						   this->hidden->mixbuf,
						   this->hidden->mixlen);

	//fprintf(stderr, "Redox audio wrinting %d to audio buffer %p with size %d\n", written, this->hidden->mixbuf, this->hidden->mixlen);
	// If we couldn't write, assume fatal error for now
	if (written != this->hidden->mixlen)
	{
		fprintf(stderr, "Error: written bytes != mixlen (%d != %d)\n", written, this->hidden->mixlen);
		this->enabled.value = 0;
	}

#ifdef DEBUG_AUDIO
	fprintf(stderr, "Wrote %d bytes of audio data\n", written);
#endif
}

static Uint8 *REDOXAUDIO_GetDeviceBuf(_THIS)
{
	return this->hidden->mixbuf;
}

static void REDOXAUDIO_CloseDevice(_THIS)
{
	if (this->hidden->mixbuf != NULL)
	{
		SDL_free(this->hidden->mixbuf);
		this->hidden->mixbuf = NULL;
	}
	if (this->hidden->output >= 0)
	{
		close(this->hidden->output);
		this->hidden->output = -1;
	}

	SDL_free(this->hidden);
	this->hidden = NULL;
}

static int REDOXAUDIO_OpenDevice(_THIS, const char *devname)
{
	SDL_PrivateAudioData *audio_data = NULL;

	fprintf(stderr, "WARNING: You are using the SDL redox audio driver!\n");

	// Open the audio device
	audio_data = (SDL_PrivateAudioData *)SDL_malloc(sizeof(SDL_PrivateAudioData));
	if (audio_data == NULL)
	{
		return SDL_OutOfMemory();
	}
	SDL_memset(audio_data, 0, sizeof(SDL_PrivateAudioData));

	this->hidden = audio_data;
	this->hidden->output = open("/scheme/audio", O_WRONLY);
	if (this->hidden->output < 0)
	{
		fprintf(stderr, "Redox audio failed to open audio\n");
		return -1;
	}

	//fprintf(stderr, "Redox audio descriptor opened %d\n", this->hidden->output);

	// Override sdl settings
	this->spec.freq = 44100;
	this->spec.format = AUDIO_S16;
	this->spec.channels = 2;
	SDL_CalculateAudioSpec(&this->spec);
	//fprintf(stderr, "Redox audio spec calculated\n");

	// Allocate mixing buffer
	this->hidden->mixlen = this->spec.size;
	this->hidden->mixbuf = (Uint8 *)SDL_malloc(this->hidden->mixlen);
	if (this->hidden->mixbuf == NULL)
	{
		fprintf(stderr, "Redox audio failed to allocate audio memory\n");
		return -1;
	}
	SDL_memset(this->hidden->mixbuf, this->spec.silence, this->spec.size);

	//fprintf(stderr, "Redox audio opened successfully\n");

	// We're ready to rock and roll. :-)
	return 0;
}

static SDL_bool REDOXAUDIO_Init(SDL_AudioDriverImpl *impl)
{
	//fprintf(stderr, "SDL redox audio init started\n");

	/* Set the function pointers */
	impl->DetectDevices = REDOXAUDIO_DetectDevices;
	impl->OpenDevice = REDOXAUDIO_OpenDevice;
	impl->WaitDevice = REDOXAUDIO_WaitDevice;
	impl->PlayDevice = REDOXAUDIO_PlayDevice;
	impl->GetDeviceBuf = REDOXAUDIO_GetDeviceBuf;
	impl->CloseDevice = REDOXAUDIO_CloseDevice;

	impl->HasCaptureSupport = SDL_FALSE;
	impl->AllowsArbitraryDeviceNames = 0;

	//fprintf(stderr, "SDL redox audio init finished\n");

	return SDL_TRUE;
}

AudioBootStrap REDOXAUDIO_bootstrap = {
	REDOXAUDIO_DRIVER_NAME, "Redox audio",
	REDOXAUDIO_Init, SDL_FALSE};
