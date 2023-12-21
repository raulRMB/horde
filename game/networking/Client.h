#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include <enet/enet.h>
#include "entt/entt.hpp"
#include "networking/base/NetMessage.h"
#include "buffers/Events_generated.h"
#include "util/defines.h"

namespace tX
{

class Client
{
public:
    ENetHost* client;
    ENetPeer* peer;
    bool connected = false;
    Client();
    void Loop();
    void Connect();
    void SendMoveTo(v2 pos, uint32_t NetworkId);
    void TriggerAbility(u32 networkId, int abilityId, v3 targeting);
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
