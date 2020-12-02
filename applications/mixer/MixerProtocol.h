#pragma once

#define AUDIO_DATA_MESSAGE_SIZE 1020

enum MixerMessageType
{
    MIXER_MESSAGE_INVALID,
    MIXER_MESSAGE_ACK,
    MIXER_MESSAGE_GREETINGS,
    MIXER_MESSAGE_AUDIODATA,
    MIXER_MESSAGE_DISCONNECT,
};

struct MixerAudioData
{
    char audiodata[AUDIO_DATA_MESSAGE_SIZE];
};

struct MixerMessage
{
    MixerMessageType type;
    union
    {
        MixerAudioData audiodata;
    };
};
