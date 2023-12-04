#include <enet/enet.h>
#include <iostream>
#include <cstring>
#include "Client.h"
#include "Game.h"
#include "NetworkDriver.h"

namespace tZ
{

Client::Client() {
    if (enet_initialize() != 0) {
        printf("Failed to initialize ENet");
    }
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (client == nullptr) {
        printf("Failed to create server");
        enet_deinitialize();
    }
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 7777;
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        printf("No available peers for initiating an ENet connection");
        enet_host_destroy(client);
        enet_deinitialize();
    }
    printf("Client Running");
}

void Client::SendMoveTo(v2 pos, uint32_t NetworkId) {
    NetMessageVector2* mt = new NetMessageVector2{};
    mt->Vector = pos;
    mt->Type = ENetMsg::MoveTo;
    mt->NetworkId = NetworkId;
    void* payload = (void*)mt;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*mt), ENET_PACKET_FLAG_RELIABLE);
    NetOutboundMessage msg = NetOutboundMessage{};
    msg.Packet = packet;
    msg.Connections.push_back(peer);
    NetworkDriver::GetOutboundQueue().push(msg);
    //delete mt;
}

void Client::Loop() {
    ENetEvent event;
    if(enet_host_service(client, &event, 2000) > 0)
    {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("CONNECT!");
        } else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            enet_uint8* dataCopy = new enet_uint8[event.packet->dataLength];
            std::memcpy(dataCopy, event.packet->data, event.packet->dataLength);
            NetworkDriver::GetInboundQueue().push(dataCopy);
            enet_packet_destroy(event.packet);
        } else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            printf("DISCONNECT!");
        }
    }
}


int gotPackets = 0;
void Client::OnInboundMessage(ENetMsg msg, enet_uint8 *data) {
    gotPackets++;
    printf("Got packets %d", gotPackets);
    switch (msg) {
    case ENetMsg::ConnectionResponse: {
            NetConnectionResponse msg = *(NetConnectionResponse *) data;
            Game::SpawnPlayer(msg.NetworkId, true);
            printf("connected owning entity networkID: %u", msg.NetworkId);
            break;
    }
    case ENetMsg::SyncTransform: {
            NetSyncTransform x = *(NetSyncTransform *) data;
            CTransform& t = Game::GetRegistry().get<CTransform>(NetworkDriver::GetNetworkedEntities().Get(x.NetworkId));
            t.Position = x.t.Position;
            t.Rotation = x.t.Rotation;
            t.Scale = x.t.Scale;
            break;
    }
    case ENetMsg::PlayerJoined: {
            NetPlayerJoined msg = *(NetPlayerJoined *) data;
            Game::SpawnPlayer(msg.NetworkId, false);
            break;
    }
    }
}

void Client::SendOutboundMessage(ENetPacket* packet) {
    enet_peer_send(peer, 0, packet);
}

void Client::Close() {
    enet_host_destroy(client);
    enet_deinitialize();
}

}