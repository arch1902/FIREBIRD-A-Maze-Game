#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include "firebird.hpp"
#include "global.hpp"
#include "client.hpp"
#include "server.hpp"
std::string network_state;
extern int socket_;

using namespace std;

int main(int argc, char **argv) {
  string full = "";
  network_state = argv[1];
  if (argc > 2){
    full = argv[2];
  }
  if (network_state != "server" && network_state != "client"){
    cout<<"Invalid Argument"<<endl;
    exit(-1);
  }
  cout<<network_state<<endl;
  const string network_mode = network_state;
  bool flag = (full == "-f") ? true : false;
  Firebird firebird(flag);
  firebird.run();
  exit(EXIT_SUCCESS);
}
