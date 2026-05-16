#include "../include/audio.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// ─────────────────────────────────────────────────────────────────────────────
// WAV builder - creates a proper WAV chunk in memory
// ─────────────────────────────────────────────────────────────────────────────
#define SAMPLE_RATE 22050

static Mix_Chunk *build_chunk(Sint16 *samples, int count)
{
    if (!samples || count <= 0)
        return NULL;

    int dataSize = count * sizeof(Sint16);
    int totalSz = 44 + dataSize;
    Uint8 *wav = (Uint8 *)malloc(totalSz);
    if (!wav)
        return NULL;

    // RIFF header
    memcpy(wav, "RIFF", 4);
    *(Uint32 *)(wav + 4) = (Uint32)(totalSz - 8);
    memcpy(wav + 8, "WAVE", 4);
    memcpy(wav + 12, "fmt ", 4);
    *(Uint32 *)(wav + 16) = 16;
    *(Uint16 *)(wav + 20) = 1; // PCM
    *(Uint16 *)(wav + 22) = 1; // Mono
    *(Uint32 *)(wav + 24) = SAMPLE_RATE;
    *(Uint32 *)(wav + 28) = SAMPLE_RATE * 2;
    *(Uint16 *)(wav + 32) = 2;
    *(Uint16 *)(wav + 34) = 16;
    memcpy(wav + 36, "data", 4);
    *(Uint32 *)(wav + 40) = (Uint32)dataSize;
    memcpy(wav + 44, samples, dataSize);

    SDL_RWops *rw = SDL_RWFromMem(wav, totalSz);
    Mix_Chunk *chunk = Mix_LoadWAV_RW(rw, 1);
    free(wav);
    return chunk;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate a SWORD SWING - rising metallic scrape
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_sword_swing(void)
{
    int ms = 180;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = 1.0f - (float)i / count;
        // Rising frequency sweep (metallic)
        float freq = 300.0f + (float)i / count * 1200.0f;
        // Mix sine + sawtooth for metallic texture
        float wave = sinf(2.0f * 3.14159f * freq * t);
        float saw = 2.0f * (freq * t - floorf(freq * t)) - 1.0f;
        float mix = (wave * 0.6f + saw * 0.3f) * fade;
        // Add high frequency shimmer
        float shimm = sinf(2.0f * 3.14159f * 3200.0f * t) * 0.1f * fade;
        buf[i] = (Sint16)((mix + shimm) * 24000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate SWORD HIT - sharp metallic clank
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_sword_hit(void)
{
    int ms = 220;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        // Exponential decay
        float fade = expf(-12.0f * t);
        // Multiple harmonics for metallic clank
        float w1 = sinf(2.0f * 3.14159f * 220.0f * t);
        float w2 = sinf(2.0f * 3.14159f * 440.0f * t) * 0.6f;
        float w3 = sinf(2.0f * 3.14159f * 880.0f * t) * 0.3f;
        float w4 = sinf(2.0f * 3.14159f * 1760.0f * t) * 0.15f;
        // Noise burst at start
        float ns = (float)(rand() % 1000 - 500) / 500.0f;
        float ns_fade = expf(-40.0f * t) * 0.4f;
        float mix = (w1 + w2 + w3 + w4) * fade * 0.5f + ns * ns_fade;
        buf[i] = (Sint16)(mix * 26000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate SWORD PICKUP - magical shimmer ascending arpeggio
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_sword_pickup(void)
{
    int ms = 400;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    // Musical notes: C5 E5 G5 C6
    float notes[] = {523.25f, 659.25f, 783.99f, 1046.50f};
    int numNotes = 4;
    int noteSamp = count / numNotes;

    int i;
    for (i = 0; i < count; i++)
    {
        int noteIdx = i / noteSamp;
        if (noteIdx >= numNotes)
            noteIdx = numNotes - 1;
        float freq = notes[noteIdx];
        float t = (float)i / SAMPLE_RATE;
        float noteTime = (float)(i % noteSamp) / noteSamp;
        float fade = (1.0f - noteTime) * 0.8f + 0.2f;
        // Bell-like tone
        float w1 = sinf(2.0f * 3.14159f * freq * t);
        float w2 = sinf(2.0f * 3.14159f * freq * 2.0f * t) * 0.4f;
        float w3 = sinf(2.0f * 3.14159f * freq * 4.0f * t) * 0.1f;
        // Overall envelope
        float env = expf(-3.0f * (float)(i % noteSamp) / noteSamp);
        buf[i] = (Sint16)((w1 + w2 + w3) * env * fade * 20000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate ENEMY DIE - descending roar explosion
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_enemy_die(void)
{
    int ms = 500;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-4.0f * t);
        // Descending frequency
        float freq = 280.0f * expf(-3.0f * t);
        float wave = sinf(2.0f * 3.14159f * freq * t);
        // Noise layer
        float ns = (float)(rand() % 1000 - 500) / 500.0f;
        float mix = wave * 0.6f + ns * 0.5f * expf(-6.0f * t);
        // Low rumble
        float rumble = sinf(2.0f * 3.14159f * 60.0f * t) * expf(-5.0f * t) * 0.4f;
        buf[i] = (Sint16)((mix + rumble) * fade * 28000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate PLAYER HURT - heavy thud + pain grunt
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_player_hurt(void)
{
    int ms = 280;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-6.0f * t);
        // Impact thud
        float thud = sinf(2.0f * 3.14159f * 80.0f * t) * expf(-8.0f * t);
        // Noise burst
        float ns = (float)(rand() % 1000 - 500) / 500.0f;
        float nfade = expf(-20.0f * t);
        // High pain note
        float pain = sinf(2.0f * 3.14159f * 320.0f * t) * expf(-10.0f * t) * 0.3f;
        float mix = thud * 0.5f + ns * nfade * 0.4f + pain;
        buf[i] = (Sint16)(mix * fade * 26000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate JUMP - springy boing
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_jump(void)
{
    int ms = 150;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-8.0f * t);
        // Rising pitch
        float freq = 180.0f + (float)i / count * 520.0f;
        float wave = sinf(2.0f * 3.14159f * freq * t);
        // Second harmonic
        float w2 = sinf(2.0f * 3.14159f * freq * 2.0f * t) * 0.25f;
        buf[i] = (Sint16)((wave + w2) * fade * 18000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate COIN PICKUP - bright sparkle ding
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_coin_pickup(void)
{
    int ms = 200;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-10.0f * t);
        // Two bell tones
        float w1 = sinf(2.0f * 3.14159f * 1200.0f * t);
        float w2 = sinf(2.0f * 3.14159f * 1600.0f * t) * 0.5f;
        float w3 = sinf(2.0f * 3.14159f * 2400.0f * t) * 0.2f;
        buf[i] = (Sint16)((w1 + w2 + w3) * fade * 16000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate POWERBALL SHOOT - electric zap charge
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_pb_shoot(void)
{
    int ms = 200;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-5.0f * t);
        // Electric buzz - fast frequency modulation
        float carrier = sinf(2.0f * 3.14159f * 400.0f * t);
        float mod = sinf(2.0f * 3.14159f * 60.0f * t);
        float fm = sinf(2.0f * 3.14159f * (400.0f + mod * 300.0f) * t);
        // Noise crackle
        float ns = (float)(rand() % 1000 - 500) / 500.0f;
        float mix = fm * 0.6f + ns * 0.15f * expf(-15.0f * t) + carrier * 0.25f;
        buf[i] = (Sint16)(mix * fade * 22000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate POWERBALL HIT - massive explosion boom
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_pb_hit(void)
{
    int ms = 600;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-3.5f * t);
        // Deep bass boom
        float bass = sinf(2.0f * 3.14159f * 55.0f * t) * expf(-4.0f * t);
        float mid = sinf(2.0f * 3.14159f * 120.0f * t) * expf(-6.0f * t) * 0.5f;
        // Explosion noise
        float ns = (float)(rand() % 1000 - 500) / 500.0f;
        float nenv = expf(-5.0f * t);
        // High crackle
        float hi = sinf(2.0f * 3.14159f * 800.0f * t) * expf(-15.0f * t) * 0.2f;
        float mix = bass + mid + ns * nenv * 0.5f + hi;
        buf[i] = (Sint16)(mix * fade * 28000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate LEVEL CLEAR - triumphant fanfare
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_level_clear(void)
{
    int ms = 800;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    // Triumphant notes: G4 C5 E5 G5 C6
    float notes[] = {392.0f, 523.25f, 659.25f, 783.99f, 1046.5f};
    int numNotes = 5;
    int noteSamp = count / numNotes;

    int i;
    for (i = 0; i < count; i++)
    {
        int ni = i / noteSamp;
        if (ni >= numNotes)
            ni = numNotes - 1;
        float freq = notes[ni];
        float t = (float)i / SAMPLE_RATE;
        float env = expf(-2.5f * (float)(i % noteSamp) / noteSamp);
        float w1 = sinf(2.0f * 3.14159f * freq * t);
        float w2 = sinf(2.0f * 3.14159f * freq * 2.0f * t) * 0.35f;
        float w3 = sinf(2.0f * 3.14159f * freq * 3.0f * t) * 0.12f;
        // Harmony
        float harm = sinf(2.0f * 3.14159f * freq * 1.5f * t) * 0.2f;
        // Global fade
        float gfade = expf(-0.8f * t);
        buf[i] = (Sint16)((w1 + w2 + w3 + harm) * env * gfade * 20000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// Generate POWERBALL PICKUP - magical orb collect
// ─────────────────────────────────────────────────────────────────────────────
static Mix_Chunk *gen_pb_pickup(void)
{
    int ms = 300;
    int count = SAMPLE_RATE * ms / 1000;
    Sint16 *buf = (Sint16 *)malloc(count * sizeof(Sint16));
    if (!buf)
        return NULL;

    int i;
    for (i = 0; i < count; i++)
    {
        float t = (float)i / SAMPLE_RATE;
        float fade = expf(-5.0f * t);
        // Swirling magical sound - frequency vibrato
        float vib = sinf(2.0f * 3.14159f * 8.0f * t) * 80.0f;
        float freq = 600.0f + vib + (float)i / count * 400.0f;
        float w1 = sinf(2.0f * 3.14159f * freq * t);
        float w2 = sinf(2.0f * 3.14159f * freq * 1.5f * t) * 0.4f;
        // Sparkle
        float sp = sinf(2.0f * 3.14159f * 2000.0f * t) * expf(-10.0f * t) * 0.3f;
        buf[i] = (Sint16)((w1 + w2 + sp) * fade * 18000.0f);
    }

    Mix_Chunk *c = build_chunk(buf, count);
    free(buf);
    return c;
}

// ═════════════════════════════════════════════════════════════════════════════
// PUBLIC API
// ═════════════════════════════════════════════════════════════════════════════

int Audio_Init(Audio *a)
{
    memset(a, 0, sizeof(Audio));

    if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16SYS, 1, 512) < 0)
    {
        printf("[Audio] Mix_OpenAudio failed: %s\n", Mix_GetError());
        return 0;
    }
    Mix_AllocateChannels(16);

    printf("[Audio] Generating procedural sound effects...\n");

    a->sword_swing = gen_sword_swing();
    a->sword_hit = gen_sword_hit();
    a->sword_pickup = gen_sword_pickup();
    a->enemy_die = gen_enemy_die();
    a->player_hurt = gen_player_hurt();
    a->jump = gen_jump();
    a->coin_pickup = gen_coin_pickup();
    a->powerball_shoot = gen_pb_shoot();
    a->powerball_hit = gen_pb_hit();
    a->level_clear = gen_level_clear();
    a->powerball_pickup = gen_pb_pickup();

    // Set volumes
    if (a->sword_swing)
        Mix_VolumeChunk(a->sword_swing, 90);
    if (a->sword_hit)
        Mix_VolumeChunk(a->sword_hit, 100);
    if (a->sword_pickup)
        Mix_VolumeChunk(a->sword_pickup, 80);
    if (a->enemy_die)
        Mix_VolumeChunk(a->enemy_die, 110);
    if (a->player_hurt)
        Mix_VolumeChunk(a->player_hurt, 100);
    if (a->jump)
        Mix_VolumeChunk(a->jump, 60);
    if (a->coin_pickup)
        Mix_VolumeChunk(a->coin_pickup, 70);
    if (a->powerball_shoot)
        Mix_VolumeChunk(a->powerball_shoot, 85);
    if (a->powerball_hit)
        Mix_VolumeChunk(a->powerball_hit, 120);
    if (a->level_clear)
        Mix_VolumeChunk(a->level_clear, 100);
    if (a->powerball_pickup)
        Mix_VolumeChunk(a->powerball_pickup, 75);

    printf("[Audio] All %d sounds generated OK.\n", 11);
    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
void Audio_Play(Mix_Chunk *chunk)
{
    if (!chunk)
        return;
    Mix_PlayChannel(-1, chunk, 0);
}

void Audio_PlayVolume(Mix_Chunk *chunk, int vol)
{
    if (!chunk)
        return;
    Mix_VolumeChunk(chunk, vol);
    Mix_PlayChannel(-1, chunk, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
void Audio_Free(Audio *a)
{
    if (!a)
        return;
    if (a->sword_swing)
        Mix_FreeChunk(a->sword_swing);
    if (a->sword_hit)
        Mix_FreeChunk(a->sword_hit);
    if (a->sword_pickup)
        Mix_FreeChunk(a->sword_pickup);
    if (a->enemy_die)
        Mix_FreeChunk(a->enemy_die);
    if (a->player_hurt)
        Mix_FreeChunk(a->player_hurt);
    if (a->jump)
        Mix_FreeChunk(a->jump);
    if (a->coin_pickup)
        Mix_FreeChunk(a->coin_pickup);
    if (a->powerball_shoot)
        Mix_FreeChunk(a->powerball_shoot);
    if (a->powerball_hit)
        Mix_FreeChunk(a->powerball_hit);
    if (a->level_clear)
        Mix_FreeChunk(a->level_clear);
    if (a->powerball_pickup)
        Mix_FreeChunk(a->powerball_pickup);
    memset(a, 0, sizeof(Audio));
}

void Audio_Quit(void)
{
    Mix_CloseAudio();
    Mix_Quit();
}