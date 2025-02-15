#pragma once

#include <libsystem/eventloop/Notifier.h>
#include <libsystem/io/Connection.h>
#include <libutils/Vector.h>

#include "mixer/MixerProtocol.h"

class Client
{
public:
    char *mixed_buffer;

    static int connected_clients;

    Notifier *notifier = nullptr;
    Connection *connection = nullptr;
    bool disconnected = false;

    // binary semaphore
    bool mixed;
    // counting semaphore
    int signals_waiting;

    Client(Connection *connection, char *mixed_buffer);

    ~Client();
    void receive_message();
    Result send_message(MixerMessage message);
};

// void client_broadcast(MixerMessage message);

// void client_destroy_disconnected();
