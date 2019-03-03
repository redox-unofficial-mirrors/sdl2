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
#include "SDL_redoxaudio.h"
#include <SDL_audio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../SDL_audiodev_c.h"

// Hidden "this" pointer for the video functions
#define _THIS SDL_AudioDevice *this

// The tag name used by REDOX audio
#define REDOXAUDIO_DRIVER_NAME "redoxaudio"

/* SDL callbacks
void (*DetectDevices) (void);
int (*OpenDevice) (_THIS, void *handle, const char *devname, int iscapture);
void (*ThreadInit) (_THIS); // Called by audio thread at start
void (*ThreadDeinit) (_THIS); // Called by audio thread at end
void (*BeginLoopIteration)(_THIS);  // Called by audio thread at top of loop
void (*WaitDevice) (_THIS);
void (*PlayDevice) (_THIS);
int (*GetPendingBytes) (_THIS);
Uint8 *(*GetDeviceBuf) (_THIS);
int (*CaptureFromDevice) (_THIS, void *buffer, int buflen);
void (*FlushCapture) (_THIS);
void (*PrepareToClose) (_THIS);  //< Called between run and draining wait for playback devices
void (*CloseDevice) (_THIS);
void (*LockDevice) (_THIS);
void (*UnlockDevice) (_THIS);
void (*FreeDeviceHandle) (void *handle);  //< SDL is done with handle from SDL_AddDeviceDevice()
void (*Deinitialize) (void);
*/

// Device driver functions
static int REDOXAUDIO_OpenDevice(_THIS, void *handle, const char *devname, int iscapture);
static void REDOXAUDIO_WaitDevice(_THIS);
static void REDOXAUDIO_PlayDevice(_THIS);
static Uint8 *REDOXAUDIO_GetDeviceBuf(_THIS);
static void REDOXAUDIO_CloseDevice(_THIS);
static void REDOXAUDIO_FreeDeviceHandle(void *handle);
static int REDOXAUDIO_Init(SDL_AudioDriverImpl *impl);

// This function waits until it is possible to write a full sound buffer
static void REDOXAUDIO_WaitDevice(_THIS)
{
}

static void REDOXAUDIO_PlayDevice(_THIS)
{
	int written = write(this->hidden->output,
						this->hidden->mixbuf,
						this->hidden->mixlen);

	// If we couldn't write, assume fatal error for now
	if ((Uint32)written != this->hidden->mixlen)
	{
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
}

static int REDOXAUDIO_OpenDevice(_THIS, void *handle, const char *devname, int iscapture)
{
	SDL_PrivateAudioData *audio_data = NULL;

	fprintf(stderr, "WARNING: You are using the SDL redox audio driver!\n");

	// Open the audio device
	audio_data = (SDL_PrivateAudioData *)SDL_calloc(1, sizeof(SDL_PrivateAudioData));
	if (audio_data == NULL)
	{
		return SDL_OutOfMemory();
	}

	this->hidden = audio_data;
	this->hidden->output = open("audio:", O_WRONLY);
	if (this->hidden->output < 0)
	{
		fprintf(stderr, "Redox audio failed to open audio:");
		return -1;
	}

	// Override sdl settings
	this->spec.freq = 44100;
	this->spec.format = AUDIO_S16;
	this->spec.channels = 2;
	SDL_CalculateAudioSpec(&this->spec);

	// Allocate mixing buffer
	this->hidden->mixlen = this->spec.size;
	this->hidden->mixbuf = (Uint8 *)SDL_malloc(this->hidden->mixlen);
	if (this->hidden->mixbuf == NULL)
	{
		fprintf(stderr, "Redox audio failed to allocate audio memory");
		return -1;
	}
	SDL_memset(this->hidden->mixbuf, this->spec.silence, this->spec.size);

	fprintf(stderr, "Redox audio opened successfully\n");

	// We're ready to rock and roll. :-)
	return 0;
}

static void REDOXAUDIO_FreeDeviceHandle(void *handle)
{
	//SDL_free(device->hidden);
	SDL_free(handle);
}

static int REDOXAUDIO_Init(SDL_AudioDriverImpl *impl)
{
	/* Set the function pointers */
	impl->OpenDevice = REDOXAUDIO_OpenDevice;
	impl->WaitDevice = REDOXAUDIO_WaitDevice;
	impl->PlayDevice = REDOXAUDIO_PlayDevice;
	impl->GetDeviceBuf = REDOXAUDIO_GetDeviceBuf;
	impl->CloseDevice = REDOXAUDIO_CloseDevice;
	impl->FreeDeviceHandle = REDOXAUDIO_FreeDeviceHandle;

	return 1;
}

AudioBootStrap REDOXAUDIO_bootstrap = {
	REDOXAUDIO_DRIVER_NAME, "Redox audio",
	REDOXAUDIO_Init, 0};