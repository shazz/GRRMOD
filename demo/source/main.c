/*===========================================
        GRRMOD
        - Test Code -
============================================*/
#include <grrlib.h>
#include <grrmod.h>

#include <stdlib.h>
#include <libgctools.h>

// Image file
#include "Impact_9_png.h"

// Mod file
#include "music_mp3.h"
#include "music_669.h"
#include "music_amf.h"
#include "music_asy.h"
#include "music_dsm.h"
#include "music_far.h"
#include "music_gdm.h"
#include "music_imf.h"
#include "music_it.h"
#include "music_med.h"
#include "music_mod.h"
#include "music_mtm.h"
#include "music_okta.h"
#include "music_s3m.h"
#include "music_stm.h"
#include "music_stx.h"
#include "music_ult.h"
#include "music_uni.h"
#include "music_xm.h"

#define MAX_WIDTH 6.0f
#define MIN_WIDTH 0.2f
#define DECAY 0.5f;

typedef struct
{
    u8 *Mem;
    u32 Size;
} PLAYLIST;

typedef struct
{
    int freq;
    int vol;
    int realvol;
    float width;
} CH;

static CH channel1 = {0, 0, 0, MIN_WIDTH};
static CH channel2 = {0, 0, 0, MIN_WIDTH};
static CH channel3 = {0, 0, 0, MIN_WIDTH};
static CH channel4 = {0, 0, 0, MIN_WIDTH};
static float calc_size(u8 voice, CH* channel);

// Callback
static void return_to_loader (void) {
    return_to_gclink("fat:/gclink.dol");
  	void (*reload)() = (void(*)()) 0x80001800;
  	reload ();
}

static void reset_cb(u32 irq, void* ctx) {
  	return_to_loader();
}


int main(int argc, char **argv) {
    float a = 0.0f;
    s16 Volume = 255;
    s8 SongNum = 0;
    PLAYLIST PlayList[] = { {(u8 *)music_mp3, music_mp3_size},
                            {(u8 *)music_669, music_669_size},
                            {(u8 *)music_amf, music_amf_size},
                            {(u8 *)music_asy, music_asy_size},
                            {(u8 *)music_dsm, music_dsm_size},
                            {(u8 *)music_far, music_far_size},
                            {(u8 *)music_gdm, music_gdm_size},
                            {(u8 *)music_imf, music_imf_size},
                            {(u8 *)music_it, music_it_size},
                            {(u8 *)music_med, music_med_size},
                            {(u8 *)music_mod, music_mod_size},
                            {(u8 *)music_mtm, music_mtm_size},
                            {(u8 *)music_okta, music_okta_size},
                            {(u8 *)music_s3m, music_s3m_size},
                            {(u8 *)music_stm, music_stm_size},
                            {(u8 *)music_stx, music_stx_size},
                            {(u8 *)music_ult, music_ult_size},
                            {(u8 *)music_uni, music_uni_size},
                            {(u8 *)music_xm, music_xm_size} };
    const u8 LastIndex = sizeof(PlayList) / sizeof(*PlayList) - 1;

    SYS_SetResetCallback(reset_cb);
    atexit(return_to_loader);

    GRRLIB_Init();
    GRRLIB_texImg *tex_Font = GRRLIB_LoadTexture(Impact_9_png);
    GRRLIB_InitTileSet(tex_Font, 10, 16, 32);

    GRRMOD_Init(true);

    GRRMOD_SetMOD(PlayList[0].Mem, PlayList[0].Size);

    PAD_Init();

    GRRLIB_Settings.antialias = true;
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    // Loop forever
    while(1) {
        GRRLIB_2dMode();
        PAD_ScanPads();  // Scan the Wiimotes

        if (PAD_ButtonsDown(0) & PAD_BUTTON_START) {
            break;
        }
        if (PAD_ButtonsHeld(0) & PAD_BUTTON_UP) {
            Volume++;
            GRRMOD_SetVolume(Volume, Volume);
        }
        if (PAD_ButtonsHeld(0) & PAD_BUTTON_DOWN) {
            Volume--;
            GRRMOD_SetVolume(Volume, Volume);
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_B) {
            GRRMOD_Pause();
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
            GRRMOD_Start();
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_X) {
            GRRMOD_Stop();
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_LEFT) {
            SongNum--;
            if(SongNum < 0) {
                SongNum = 0;
            }
            GRRMOD_Unload();
            GRRMOD_SetMOD(PlayList[SongNum].Mem, PlayList[SongNum].Size);
            GRRMOD_Start();
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT) {
            SongNum++;
            if(SongNum > LastIndex) {
                SongNum = LastIndex;
            }
            GRRMOD_Unload();
            GRRMOD_SetMOD(PlayList[SongNum].Mem, PlayList[SongNum].Size);
            GRRMOD_Start();
        }

        GRRLIB_Camera3dSettings(0.0f, 0.0f,13.0f, 0,1,0, 0,0,0);

        GRRLIB_SetLightAmbient(0x333333FF);
        GRRLIB_SetLightDiff(0,(guVector){0.0f,0.0f,0.0f},20.0f,1.0f,0x00FFFFFF);
        GRRLIB_SetLightDiff(1,(guVector){0.0f,13.0f,3.0f},20.0f,1.0f,0xFF00FFFF);
        GRRLIB_SetLightDiff(2,(guVector){0.0f,-13.0f,3.0f},20.0f,1.0f,0xFFFF00FF);
        GRRLIB_SetLightDiff(3,(guVector){13.0f,0.0f,3.0f},20.0f,1.0f,0xFF0000FF);
        GRRLIB_SetLightDiff(4,(guVector){-13.0f,0.0f,3.0f},20.0f,1.0f,0x00FF00FF);

        GRRLIB_3dMode(0.1,1000,45,0,1);

        GRRLIB_ObjectViewInv(-3.0f, 0.0f, 0.0f, a,a*2,a*3, 1.0f,calc_size(0, &channel1),1.0f);
        GRRLIB_DrawCube(1.0,true,0xFFFFFFFF);

        GRRLIB_ObjectViewInv(-1.0f, 0.0f, 0.0f, a,a*2,a*3, 1.0f,calc_size(1, &channel2),1.0f);
        GRRLIB_DrawCube(1.0,true,0xFFFFFFFF);

        GRRLIB_ObjectViewInv(1.0f, 0.0f, 0.0f, a,a*2,a*3, 1.0f,calc_size(2, &channel3),1.0f);
        GRRLIB_DrawCube(1.0,true,0xFFFFFFFF);

        GRRLIB_ObjectViewInv(3.0f, 0.0f, 0.0f, a,a*2,a*3, 1.0f,calc_size(3, &channel4),1.0f);
        GRRLIB_DrawCube(1.0,true,0xFFFFFFFF);


        a+=0.5f;
        GRRLIB_2dMode();
        GRRLIB_Printf(10, 10, tex_Font, 0xFFFFFFFF, 1, "Song: %s", GRRMOD_GetSongTitle());
        GRRLIB_Printf(10, 26, tex_Font, 0xFFFFFFFF, 1, "Type: %s", GRRMOD_GetModType());
        GRRLIB_Printf(10, 42, tex_Font, 0xFFFFFFFF, 1, "A:Play - X:Stop - B: Pause - L/R: Prev/Next - U/D: Volume");

#ifdef _GRRMOD_DEBUG
        GRRLIB_Printf(10, 58, tex_Font, 0xFFFFFFFF, 1, "Mixing Time: %04d", GRRMOD_MixingTime());
#endif

        GRRLIB_Render();  // Render the frame buffer to the TV
    }

    GRRMOD_End();
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    return_to_gclink("fat:/gclink.dol");

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

static float calc_size(u8 voice, CH* channel) {
    int freq = GRRMOD_GetVoiceFrequency(voice);
    int vol = GRRMOD_GetVoiceVolume(voice);
    int realvol = GRRMOD_GetRealVoiceVolume(voice);

    if (freq != channel->freq || vol != channel->vol || realvol > channel->realvol) {
        channel->width = MAX_WIDTH;
    }
    else {
        channel->width -= DECAY;
        if (channel->width < MIN_WIDTH) {
            channel->width = MIN_WIDTH;
        }
    }

    channel->vol = vol;
    channel->freq = freq;
    channel->realvol = realvol;

    return channel->width;
}
