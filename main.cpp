#include "game/Game.h"
#include <cstring>

int main(int argc, char* argv[])
{
    tZ::Game::Instance();

    if(argc <= 1) {
        return tZ::Game::Instance().Run(false);
    }
    else {
       return tZ::Game::Instance().Run(std::strcmp(argv[1], "-s") == 0);
    }
}
