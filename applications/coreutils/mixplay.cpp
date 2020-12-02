#include "applications/mixer/MixerProtocol.h"
#include "kernel/drivers/AC97.h"
#include <libsystem/io/Connection.h>
#include <libsystem/io/File.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Socket.h>
#include <libsystem/io/Stream.h>
#include <libutils/Path.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        stream_format(err_stream, "%s: Missing Audio file operand\n", argv[0]);
        return PROCESS_FAILURE;
    }
    __cleanup(stream_cleanup) Stream *streamin = stream_open(argv[1], OPEN_READ);

    if (handle_has_error(streamin))
    {
        return handle_get_error(streamin);
    }

    __cleanup(stream_cleanup) Stream *streamout = stream_open("/Devices/sound", OPEN_WRITE | OPEN_CREATE);

    if (handle_has_error(streamout))
    {
        return handle_get_error(streamout);
    }

    size_t read;
    char buffer[AUDIO_DATA_MESSAGE_SIZE];

    // start socket connection
    Connection *mixer_connection = socket_connect("/Session/mixer.ipc");
    // receive greeting message
    MixerMessage message = {};
    size_t message_size = connection_receive(mixer_connection, &message, sizeof(MixerMessage));

    if (handle_has_error(mixer_connection))
    {
        printf("Connection error: %s!", handle_error_string(mixer_connection));

        // client_destroy_disconnected();
        return ERR_CONNECTION_REFUSED;
    }
    if (message_size != sizeof(MixerMessage))
    {

        // client_destroy_disconnected();
        connection_close(mixer_connection);
        printf("\ngot greeting message of wrong size");
        return PROCESS_FAILURE;
    }

    // start playing
    if (message.type == MIXER_MESSAGE_GREETINGS)
    {
        while ((read = stream_read(streamin, &buffer, AUDIO_DATA_MESSAGE_SIZE)) != 0)
        {
            if (handle_has_error(streamin))
            {
                return handle_get_error(streamin);
            }
            // start sending message
            message.type = MIXER_MESSAGE_AUDIODATA;
            strlcpy(message.audiodata.audiodata, buffer, AUDIO_DATA_MESSAGE_SIZE);
            // printf("%d \n", sizeof(message));
            // printf("message sent to %08x\n", mixer_connection);
            // printf("message is : %s ,%s", message.audiodata.audiodata, buffer);
            connection_send(mixer_connection, &message, sizeof(message));

            // if (handle_has_error(streamout))
            // {
            //     return handle_get_error(streamout);
            // }
        }
    }
    // send disconnect message
    message.type = MIXER_MESSAGE_DISCONNECT;
    strlcpy(message.audiodata.audiodata, "\0", AUDIO_DATA_MESSAGE_SIZE);
    // disconnect
    connection_close(mixer_connection);

    printf("\nFinish Playing");

    return PROCESS_SUCCESS;
}
