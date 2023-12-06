#include "NetworkDriver.h"

#include <thread>

#include "components/Follow.h"
#include "networking/buffers/Events_generated.h"

namespace tZ
{

NetworkDriver::NetworkDriver(){}

NetworkDriver& NetworkDriver::Instance()
{
    static NetworkDriver instance;
    return instance;
}

void NetworkDriver::SetIsServer(bool isServer) {
    Instance().isServer = isServer;
}

void NetworkDriver::Init(long long periodMicroseconds) {
    if(NetworkDriver::IsServer()) {
        server = new Server();
        std::thread networkThread([]()
        {
            while(1)
            {
                NetworkDriver::GetServer()->Loop();
            }
        });
        networkThread.detach();
    } else {
        client = new Client();
        std::thread networkThread([periodMicroseconds]()
        {
            while(1)
            {
                NetworkDriver::GetClient()->Loop();
            }
        });
        networkThread.detach();
    }
}

void NetworkDriver::ProcessQueues() {
    while(!GetInboundQueue().empty()) {
        auto msg = GetInboundQueue().front();
        auto header = Net::GetHeader(msg.data);
        if(IsServer()) {
            Instance().server->OnInboundMessage(header, msg.peer);
        } else {
            Instance().client->OnInboundMessage(header);
        }
        GetInboundQueue().pop();
    }
    while(!GetOutboundQueue().empty()) {
        NetOutboundMessage som = GetOutboundQueue().front();
        if(IsServer()) {
            Instance().server->SendOutboundMessage(som);
        } else {
            Instance().client->SendOutboundMessage(som.Packet);
        }
        GetOutboundQueue().pop();
    }
    if(IsServer()) {
        Instance().server->flush();
    } else {
        Instance().client->flush();
    }
}

NetworkedEntities& NetworkDriver::GetNetworkedEntities() {
    return Instance().networkedEntities;
}

Server* NetworkDriver::GetServer() {
    return Instance().server;
}

Client* NetworkDriver::GetClient() {
    return Instance().client;
}

bool NetworkDriver::IsServer() {
    return Instance().isServer;
}

bool NetworkDriver::IsOfflineMode() {
    return Instance().offlineMode;
}
}