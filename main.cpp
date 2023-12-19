#include "game/Game.h"
#include <cstring>

int main(int argc, char* argv[])
{
    if(argc <= 1) {
        return tX::Game::Instance().Run(tX::EngineMode::Standalone);
    }
    else if(std::strcmp(argv[1], "-s") == 0) {
       return tX::Game::Instance().Run(tX::EngineMode::Server);
    }
    else if(std::strcmp(argv[1], "-c") == 0) {
        return tX::Game::Instance().Run(tX::EngineMode::Client);
    }
}
