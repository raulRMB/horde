#include "game/Game.h"
#include <CLI11.hpp>
#include "game/networking/base/NetworkDriver.h"

int main(int argc, char** argv)
{
    CLI::App app{"Horde"};

    bool server = false;
    bool client = false;

    int port = 7777;
    std::string ip = "127.0.0.1";

    app.add_flag("-s,--server", server, "Run in server mode");
    app.add_flag("-c,--client", client, "Run in client mode");

    app.add_option("-p, --port", port, "Port to run on/connect to");
    app.add_option("-i, --ip-address", ip, "IP Address to connect to (client mode only)");

    CLI11_PARSE(app, argc, argv);

    tX::NetworkDriver::SetPort(port);
    tX::NetworkDriver::SetIP(ip);
    if(server) {
        return tX::Game::Instance().Run(tX::EngineMode::Server);
    }
    else if(client) {
        return tX::Game::Instance().Run(tX::EngineMode::Client);
    }
    else {
        return tX::Game::Instance().Run(tX::EngineMode::Standalone);
    }

}
