#include "game/Game.h"
#include <CLI11.hpp>

int main(int argc, char** argv)
{
    CLI::App app{"Horde"};

    bool server = false;
    bool client = false;

    app.add_flag("-s,--server", server, "Run in server mode");
    app.add_flag("-c,--client", client, "Run in client mode");

    CLI11_PARSE(app, argc, argv);

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
