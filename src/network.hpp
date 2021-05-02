#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
#include <player.hpp>
#include <vetor>

class network{
    SDLNet_SocketSet server;
    TCPsocket connection;
    char tmp[1400];
    public :
        network(const char* ip);
        ~network();
        void send(Player* p);
        void recv(vector<Player>* enemies)
}