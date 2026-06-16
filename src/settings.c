#include "settings.h"

#include <SDL2/SDL_mixer.h>

#define MUSIC_CHANNEL 0
#define MUSIC_VOLUME (int)(MIX_MAX_VOLUME * 0.7)

static int muted = 0;
static int musicOn = 1;
static int sfxOn = 1;


void initSettings(void) {
    muted = 0;
    musicOn = 1;
    sfxOn = 1;
}

static void refreshVolumes(void) {
    // Canal musique
    Mix_Volume(MUSIC_CHANNEL, (muted || !musicOn) ? 0 : MUSIC_VOLUME);

    // Tous les autres canaux (effets)
    int sfxVolume = (muted || !sfxOn) ? 0 : MIX_MAX_VOLUME;
    for (int channel = 1; channel < 255; channel++) {
        Mix_Volume(channel, sfxVolume);
    }
}

void applyAudioSettings(Mix_Chunk **sounds) {
    (void)sounds;
    refreshVolumes();
}

void toggleMute(Mix_Chunk **sounds) {
    (void)sounds;
    muted = !muted;
    refreshVolumes();
}

int isMuted(void) {
    return muted;
}

void toggleMusic(Mix_Chunk **sounds) {
    musicOn = !musicOn;
    if (musicOn && !muted) {
        // Relance la musique si elle ne tourne plus
        if (!Mix_Playing(MUSIC_CHANNEL) && sounds && sounds[0]) {
            Mix_PlayChannel(MUSIC_CHANNEL, sounds[0], -1);
        }
    }
    refreshVolumes();
}

int isMusicOn(void) {
    return musicOn;
}

void toggleSfx(void) {
    sfxOn = !sfxOn;
    refreshVolumes();
}

int isSfxOn(void) {
    return sfxOn;
}
