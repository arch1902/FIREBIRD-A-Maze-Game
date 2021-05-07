#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
#include "player.hpp"
#include "player2.hpp"
#include <vector>

class network{
    SDLNet_SocketSet server;
    TCPsocket connection;
    char tmp[1400];
    public :
        network(const char* ip);
        ~network();
        void send(Player* p1);
        void recv(player2* p2,Player* p1);
};