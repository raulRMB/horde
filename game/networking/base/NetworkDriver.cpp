#include "NetworkDriver.h"

#include <thread>

#include "components/CFollow.h"
#include "networking/buffers/Events_generated.h"
#include "Game.h"

namespace tX
{

NetworkDriver::NetworkDriver(){}

NetworkDriver& NetworkDriver::Instance()
{
    static NetworkDriver instance;
    return instance;
}

void NetworkDriver::Init(long long periodMicroseconds) {
    if(Game::IsServer()) {
        server = new class Server();
        std::thread networkThread([]()
        {
            while(1)
            {
                NetworkDriver::GetServer()->Loop();
            }
        });
        networkThread.detach();
    } else {
        client = new class Client();
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
    int inboundProcessed = 0;
    int outboundProcessed = 0;
    while(!GetInboundQueue().empty()) {
        inboundProcessed += 1;
        auto msg = GetInboundQueue().front();
        auto header = Net::GetHeader(msg.data);
        if(Game::IsServer()) {
            Instance().server->OnInboundMessage(header, msg.peer);
        } else {
            Instance().client->OnInboundMessage(header);
        }
        GetInboundQueue().pop();
    }
    while(!GetOutboundQueue().empty()) {
        outboundProcessed += 1;
        OutboundMessage som = GetOutboundQueue().front();
        if(Game::IsServer()) {
            Instance().server->SendOutboundMessage(som);
        } else {
            Instance().client->SendOutboundMessage(som.Packet);
        }
        GetOutboundQueue().pop();
    }
    if(inboundProcessed > 0 || outboundProcessed > 0) {
        if (Game::IsServer()) {
            Instance().server->flush();
        } else {
            Instance().client->flush();
        }
    }
}

NetworkedEntities& NetworkDriver::GetNetworkedEntities() {
    return Instance().networkedEntities;
}

class Server* NetworkDriver::GetServer() {
    return Instance().server;
}

class Client* NetworkDriver::GetClient() {
    return Instance().client;
}

}