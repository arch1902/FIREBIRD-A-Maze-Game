#include "network.hpp"

network::network(const char* ipchar){
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
    SDLNet_TCP_Close(connection);
    SDLNet_FreeSocketSet(server);
    SDLNet_Quit();
}
void network::send(Player* p){
    if(p->isReady()){
        // 1 id curframe pos block nextblock dir anc anw life score damages powermode
        sprintf(tmp,"1 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
                    p->getId(),p->getCurframe(),
                    p->get_pos().x,p->get_pos().y,
                    p->get_block().x,p->get_block().y,
                    p->get_nextblock().x,p->get_nextblock().y,
                    p->get_dir(),p->get_anc(),p->get_anw(),
                    p->get_life(),p->get_score(),p->get_damaged(),
                    p->get_power_mode());
        int size=0;
        int len = strlen(tmp)+1;
        while(size<len){
            size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
        }  
        cout<<"sent "<<tmp<<"\n";          
    }
}
void network::recv(Player* p2,Player* p1){
    cout<<"FF "<<SDLNet_CheckSockets(server,0)<<SDLNet_SocketReady(connection)<<endl;
    while(SDLNet_CheckSockets(server,0)>0 && SDLNet_SocketReady(connection)){
        int offset = 0;
        do{
            //cout<<"here: "<<offset<<" "<<tmp[strlen(tmp)-1]<<endl;
            offset+=SDLNet_TCP_Recv(connection,tmp+offset,1400);
            if(offset<=0)
                return;
            
        }while(tmp[strlen(tmp)-1]!='\n');
        int type,id;
        cout<<"Received "<<tmp<<endl;
        sscanf(tmp,"%d %d",&type,&id);
        cout<<"type,id: "<<type<<","<<id<<endl;
        if(type==0){
            p1->setId(id);
        }else if (type==1){
            
            int tmp2,curframe,posx,posy,blkx,blky,nxtblkx,nxtblky,dir,anc,anw,life,score,damaged,powermode;
            // 1 id curframe pos block nextblock dir anc anw life score damaged powermode
            sscanf(tmp,"1 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
            &tmp2,&curframe,&posx,&posy,
            &blkx,&blky,
            &nxtblkx,&nxtblky,
            &dir,&anc,&anw,
            &life,&score,&damaged,
            &powermode);
            p2->set_pos(Point{posx,posy});
            p2->set_block(Point{blkx,blky});
            p2->set_nextblock(Point{nxtblkx,nxtblky});
            p2->set_dir(dir);
            p2->set_anc(anc);
            p2->set_anw(anw);
            p2->set_life(life);
            p2->set_score(score);
            p2->set_damaged(damaged);
            p2->set_power_mode(powermode);
            cout<<"recv\n"; 
            break;

        }

        }
    }
    

