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
        if(Instance().useOwnThread) {
            std::thread networkThread([]() {
                while (1) {
                    NetworkDriver::GetServer()->Loop();
                    while (!GetOutboundQueue().empty()) {
                        OutboundMessage som = GetOutboundQueue().front();
                        if (som.Packet != nullptr) {
                            Instance().server->SendOutboundMessage(som);
                        }
                        GetOutboundQueue().pop();
                        Instance().server->flush();
                    }
                }
            });
            networkThread.detach();
        }
    } else if(Game::IsClient()) {
        client = new class Client();
        if(Instance().useOwnThread) {
            std::thread networkThread([]() {
                while (1) {
                    NetworkDriver::GetClient()->Loop();
                    while (!GetOutboundQueue().empty()) {
                        OutboundMessage som = GetOutboundQueue().front();
                        if (som.Packet != nullptr) {
                            Instance().client->SendOutboundMessage(som.Packet);
                        }
                        GetOutboundQueue().pop();
                        Instance().client->flush();
                    }
                }
            });
            networkThread.detach();
        }
    }
}

void NetworkDriver::Process() {
    if(!Instance().useOwnThread) {
        if (Game::IsServer()) {
            NetworkDriver::GetServer()->Loop();
        } else {
            NetworkDriver::GetClient()->Loop();
        }
    }
    while(!GetInboundQueue().empty()) {
        auto msg = GetInboundQueue().front();
        if(msg.data == nullptr) {
            GetInboundQueue().pop();
            continue;
        }
        auto header = Net::GetHeader(msg.data);
        if(Game::IsServer()) {
            Instance().server->OnInboundMessage(header, msg.peer);
        } else {
            Instance().client->OnInboundMessage(header);
        }
        GetInboundQueue().pop();
    }
    if(!Instance().useOwnThread) {
        while (!GetOutboundQueue().empty()) {
            OutboundMessage som = GetOutboundQueue().front();
            if (som.Packet != nullptr) {
                if (Game::IsServer()) {
                    Instance().server->SendOutboundMessage(som);
                } else {
                    Instance().client->SendOutboundMessage(som.Packet);
                }
            }
            GetOutboundQueue().pop();
        }
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