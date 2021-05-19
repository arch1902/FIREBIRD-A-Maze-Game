#pragma once

#include <math.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>
#include <vector>
#include <ctime>

vector<vector<string> > maze;
vector<vector<int> > visited;

int is_loop(int x, int y){
    visited[x][y] = 1;
    int ans = 1;
    if (x-1>=0 && visited[x-1][y] == 0 && maze[x-1][y] == "."){
        ans+= is_loop(x-1,y);
    }
    if (y-1>=0 && visited[x][y-1] == 0 && maze[x][y-1] == "."){
        ans+= is_loop(x,y-1);
    }
    if (x+1<24 && visited[x+1][y] == 0 && maze[x+1][y] == "."){
        ans+= is_loop(x+1,y);
    }
    if (y+1<24 && visited[x][y+1] == 0 && maze[x][y+1] == "."){
        ans+= is_loop(x,y+1);
    }
    return ans;
}


vector<string> generator(){
    srand( time(NULL));
    vector<vector<string>> t1 = maze;
    vector<vector<int> > t2 = visited;
    vector<string> temp;
    vector< vector<int> >temp3;
    vector<int> temp2;
    for(int i = 0;i<24;i++){
        temp2.push_back(0);
    }
    for(int i = 0;i<24;i++){
        temp3.push_back(temp2);
    }
    visited = temp3;
    for(int i = 0;i<24;i++){
        temp.push_back(".");
    }
    for(int i = 0;i<24;i++){
        maze.push_back(temp);
    }
    for (int i = 0;i<24;i++){
        maze[0][i] = "#";
        maze[23][i] = "#";
        if (i == 12){continue;}
        maze[i][0] = "#";
        maze[i][23] = "#";  
    }
    for(int i = 0;i < 3 ;i++){
        for (int j = 0;j<8;j++){
            maze[11+i][8+j] = "#";
        }
    }
    maze[11][1] = "#";
    // maze[11][2] = "#";
    // maze[11][3] = "#";
    maze[11][22] = "#";
    // maze[11][21] = "#";
    // maze[11][20] = "#"; 
    maze[13][1] = "#";
    // maze[13][2] = "#";
    // maze[13][3] = "#";
    maze[13][22] = "#";
    // maze[13][21] = "#";
    // maze[13][20] = "#";


    // for (auto j: maze){
    //     for (auto i :j){
    //         cout<<i;
    //     }
    //     cout<<endl;
    // }
    // cout<<"------------"<<endl;
    // for (auto j: visited){
    //     for (auto i :j){
    //         cout<<i;
    //     }
    //     cout<<endl;
    // }
    int j = 0;
    while (j<140){
        int x = rand()%22 + 1;
        int y = rand()%22 + 1;
        //cout<<x<<" "<<y<<endl;
        if (maze[x][y] == "#"){continue;}
        maze[x][y] = "#";
        visited = temp3;
        //cout<<is_loop(12,0)<<endl;
        if(is_loop(12,0) != 457-j){
            maze[x][y] = ".";
            continue;
        }
        //cout<<j<<endl;
        j+=1;
    }

    maze[10][11] = ".";
    maze[10][12] = ".";

    string out1;

    maze[11][11] = ".";// y = 11 x = 12
    maze[12][11] = ".";
    for (auto j: maze){
        for (auto i :j){
            out1 += i;
            //cout<<i;
        }
        // out += "\"";
        // out += "\n";
        //cout<<endl;
    }
    maze[11][11] = "#";
    maze[12][11] = "#";
    int  q = 0;
    while(q<4){
        int x = rand()%22 + 1;
        int y = rand()%22 + 1;
        if(maze[x][y]== "#"){continue;}
        //cout<<q<<endl;
        //cout<<x<<" "<<y<<endl;
        maze[x][y] = "C";
        q=q+1;        
    }
    // maze[12][1] = "=";
    // maze[12][2] = "=";
    // maze[12][3] = "=";
    // maze[12][22] = "=";
    // maze[12][21] = "=";
    // maze[12][20] = "="; 



    maze[12][0] = "L";
    maze[12][23] = "R"; 
    maze[18][9] = "P";
    maze[11][11] = "E";
    maze[11][12] = "E";
    maze[12][11] = "E";
    maze[12][12] = "E";
    maze[12][10] = "E";
    maze[12][13] = "E";
    // maze[12][0] = "#";
    // maze[12][23] = "#";
    string out2;
    for (auto j: maze){
        for (auto i :j){
            out2 += i;
            //cout<<i;
        }
        // out += "\"";
        // out += "\n";
        //cout<<endl;
    }
    //cout <<out;
    maze = t1;
    visited = t2;

    vector<string> out;
    out.push_back(out2);
    out.push_back(out1);

    return out;
}

// #,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,
// #,#,.,.,#,.,.,.,.,.,.,.,#,#,.,.,.,#,#,.,.,.,.,#,
// #,.,.,.,.,#,.,#,#,.,#,.,#,.,.,.,.,.,.,.,.,.,#,#,
// #,.,#,.,.,.,.,.,#,.,.,.,.,.,#,.,.,#,#,.,#,.,#,#,
// #,#,#,.,.,.,.,.,.,#,.,.,#,.,#,#,#,.,.,#,#,.,#,#,
// #,.,.,.,.,#,#,.,.,.,.,#,#,.,#,.,.,.,#,.,.,.,.,#,
// #,#,.,#,.,.,.,.,.,.,.,#,.,#,.,.,.,.,#,#,.,#,.,#,
// #,#,#,.,#,.,#,.,#,#,.,.,.,.,#,.,.,#,.,.,.,#,#,#,
// #,#,.,.,#,#,#,.,#,.,.,.,.,#,#,#,.,#,.,#,.,.,.,#,
// #,.,.,.,.,.,.,.,.,#,#,.,.,.,#,.,.,.,.,.,#,.,.,#,
// #,#,.,#,#,#,.,.,.,.,#,?,?,.,#,.,.,#,.,.,#,.,#,#,
// #,#,#,#,.,.,.,.,#,#,#,2,3,#,#,#,.,.,.,.,.,.,#,#,
// .,.,.,.,#,#,#,.,#,#,#,1,#,#,#,#,#,#,.,#,.,.,.,.,
// #,#,.,.,#,.,.,.,#,#,#,#,#,#,#,#,#,#,.,.,#,.,#,#,
// #,.,.,.,.,.,#,.,#,#,.,.,.,#,.,.,.,.,.,#,#,.,.,#,
// #,.,#,.,.,.,.,#,.,.,#,.,.,.,.,.,.,.,.,.,.,#,#,#,
// #,#,#,.,.,#,.,.,.,.,#,.,.,.,.,#,.,.,.,.,#,.,.,#,
// #,#,.,.,.,#,.,#,#,.,.,#,#,#,.,#,.,.,.,.,#,.,.,#,
// #,#,#,.,.,.,#,#,.,P,#,#,.,#,#,.,.,.,.,#,.,#,.,#,
// #,.,.,.,#,#,.,.,.,.,#,.,.,#,.,#,#,.,.,.,.,.,.,#,
// #,#,.,#,#,#,#,#,.,.,.,.,.,.,.,.,.,#,#,.,.,.,.,#,
// #,.,.,.,#,.,.,.,#,.,.,.,#,.,.,.,.,.,.,#,.,.,.,#,
// #,#,.,.,.,.,.,.,#,#,#,.,.,.,.,#,.,.,.,#,#,.,.,#,
// #,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,







// #,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,
// #,#,.,.,#,.,.,.,.,.,.,.,#,#,.,.,.,#,#,.,.,.,.,#,
// #,.,C,.,.,#,.,#,#,.,#,.,#,.,.,.,.,.,.,.,.,.,#,#,
// #,.,#,.,.,.,.,.,#,.,.,.,.,.,#,.,.,#,#,.,#,.,#,#,
// #,#,#,.,.,.,.,.,.,#,.,.,#,.,#,#,#,.,.,#,#,.,#,#,
// #,.,.,.,.,#,#,.,.,.,.,#,#,.,#,.,.,.,#,.,.,.,.,#,
// #,#,.,#,.,.,.,.,.,.,.,#,.,#,.,.,.,.,#,#,C,#,.,#,
// #,#,#,.,#,.,#,.,#,#,.,.,.,.,#,.,.,#,.,.,.,#,#,#,
// #,#,.,.,#,#,#,.,#,.,.,.,.,#,#,#,.,#,.,#,.,.,.,#,
// #,.,.,.,.,.,.,.,.,#,#,.,.,.,#,.,.,.,.,.,#,.,C,#,
// #,#,.,#,#,#,.,.,.,.,#,.,.,.,#,.,.,#,.,.,#,.,#,#,
// #,#,#,#,.,.,.,.,#,#,#,E,E,#,#,#,.,.,.,.,.,.,#,#,
// L,.,.,.,#,#,#,.,#,#,E,E,E,E,#,#,#,#,.,#,.,.,.,R,
// #,#,.,.,#,.,.,.,#,#,#,#,#,#,#,#,#,#,.,.,#,.,#,#,
// #,.,.,.,.,.,#,.,#,#,.,.,.,#,.,.,.,.,.,#,#,.,.,#,
// #,.,#,.,.,.,.,#,.,.,#,.,.,.,.,.,.,.,.,.,.,#,#,#,
// #,#,#,.,.,#,.,.,.,.,#,.,.,.,.,#,.,.,.,.,#,.,.,#,
// #,#,.,.,.,#,.,#,#,.,.,#,#,#,.,#,.,.,.,.,#,.,.,#,
// #,#,#,.,.,.,#,#,.,P,#,#,.,#,#,.,.,.,.,#,.,#,.,#,
// #,.,.,.,#,#,.,.,.,.,#,.,.,#,.,#,#,.,.,.,.,.,.,#,
// #,#,.,#,#,#,#,#,.,.,.,.,.,.,.,.,.,#,#,.,.,.,.,#,
// #,.,.,.,#,.,.,.,#,.,.,.,#,.,.,.,.,.,.,#,.,.,.,#,
// #,#,.,.,.,.,.,.,#,#,#,.,.,.,.,#,C,.,.,#,#,.,.,#,
// #,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,#,