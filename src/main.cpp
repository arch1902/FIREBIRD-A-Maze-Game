#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include "pacman.hpp"
#include "global.hpp"
#include "client.hpp"
#include "server.hpp"

std::string network_state;
int socket_;


using namespace std;

bool parse_options(const int argc, char **argv)  {
  bool fullscreen_mode = false;
  opterr = 0;
  const option long_options[] = {
      {"fullscreen", no_argument, nullptr, 'f'},
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {nullptr, 0, nullptr, 0},
  };

  while(true) {
    const int curind = optind;
    const int c = getopt_long(argc, argv, "fhv", long_options, nullptr);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        cout << R"(Usage: pacman-sdl [options]

Options:
    -f  --fullscreen    fullscreen mode
    -h, --help          print this help menu
)";
        exit(EXIT_SUCCESS);
      case 'f':
        fullscreen_mode = true;
        break;
      case '?': {
        string av(argv[curind]);
        int n = 0;
        while (av[n] == '-') {
          ++n;
        }
        av.erase(av.begin(), av.begin() + n);
        cerr << "Unrecognized option: '" << av << "'\n";
        exit(EXIT_FAILURE);
      }
      default:
        break;
    }
  }

  return fullscreen_mode;
}

int main(int argc, char **argv) {

  //bool is_fullscreen = parse_options(argc, argv);
  network_state = argv[1];
  cout<<network_state<<endl;
  if (network_state=="server"){
    socket_ = start_server();
  }else if(network_state == "client"){
    socket_ = connect_client();
  }else{
    cout<<"Invalid Argument"<<endl;
    exit(-1);
  }

  if (network_state=="server"){
    send_from_server("hey_there",socket_);
    string xyz = receive_in_server(socket_);
    cout<<"Received "<<xyz<<endl;
  }else{
    send_from_client("hello_server",socket_);
    string abc = receive_in_client(socket_);
    cout<<"Received "<<abc<<endl;
  }
  const string network_mode = network_state;
  Pacman pacman(false);
  pacman.run();
  exit(EXIT_SUCCESS);
}
