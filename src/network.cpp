#include "network.hpp"

network::network(const char* ip){
    SDLNet_Init();
    IPaddress ip;
    if(SDLNet_ResolveHost(&ip,ipchar,1234)==-1){
        std::cout<<"error";
    }
    connection = SDLNet_TCP_Open(&ip);
    if(connection==NULL)
        std::cout<<"bad ip";
    server = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(server,connection);

};
network::~network(){
    SDLNet_TCP_Send(connection,"2 \n",4);
    SDLNet_TCP_Close();
    SDLNet_FreeSocketSet(server);
    SDLNet_Quit();
};
void network::send(Player* p);
void network::recv(vector<Player>* enemies)
