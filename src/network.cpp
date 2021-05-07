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
}
void network::send(Player* p){
    if(p->isReady()){
        // 1 id curframe pos block nextblock dir anc anw life score damages powermode
        sprintf(tmp,"1 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
                    p->getId(),p->getCurframe(),
                    p->get_pos().x,p->get_pos().y,
                    p->get_nextblock().x,p->get_nextblock().y,
                    p->get_dir(),p->get_anc(),p->get_anw(),
                    p->get_life(),p->get_score(),p->get_damaged(),
                    p->get_power_mode());
        int size=0;
        int len = strlen(tmp)+1;
        while(size<len){
            size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
        }            
    }
}
void network::recv(player2* p2,Player* p1){
    while(SDLNet_CheckSockets(socket,0)>0 && SDLNet_SocketReady(connectiom)){
        int offset = 0;
        do{
            offset+=SDLNet_TCP_Recv(connection,tmp+offset,1400);
            if(offset<=0)
                return;
            
        }while(tmp[strlen(tmp)-1]!='\0');
        int type,id;
        sscanf(tmp,"%d %d",&type,&id);
        if(type==0){
            p1->setId(id);
        }else if (type==1){
            
            int tmp2;
            // 1 id curframe pos block nextblock dir anc anw life score damages powermode
            sscanf(tmp,"1 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
            &tmp2,&(p2->curframe),p2->get_pos().x,p2->get_pos().y,
            p2->get_block().x,p2->get_block().y,
            p2->get_nextblock().x,p2->get_nextblock().y,
            p2->get_dir(),p2->get_anc(),p2->get_anw(),
            p2->get_life(),p2->get_score(),p2->get_damaged(),
            p2->get_power_mode());
            break;

        }

        }
    }
    

