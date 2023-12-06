#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include <enet/enet.h>
#include "entt/entt.hpp"
#include "NetMessage.h"
#include "buffers/Events_generated.h"
#include "defines.h"

namespace tZ
{

class Client {
public:
    ENetHost* client;
    ENetPeer* peer;
    Client();
    void Loop();
    void SendMoveTo(v2 pos, uint32_t NetworkId);
    void OnInboundMessage(const Net::Header* header);
    void SendInitialConnection();
    void SendOutboundMessage(ENetPacket* packet);
    void flush() { enet_host_flush(client); }
    void Close();
private:
    void Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data);

};

}

#endif
