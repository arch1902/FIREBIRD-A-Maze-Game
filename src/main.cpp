#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include "pacman.hpp"

using namespace std;

pair<bool, bool> parse_options(const int argc, char **argv)  {
  bool fullscreen_mode = false;
  bool debug_mode = false;
  opterr = 0;
  const option long_options[] = {
      {"fullscreen", no_argument, nullptr, 'f'},
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {nullptr, 0, nullptr, 0},
  };

  for (;;) {
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
      case 'd':
        debug_mode = true;
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
        // do nothing
        break;
    }
  }

  return make_pair(fullscreen_mode, false);
}

int main(int argc, char **argv) {

  pair<bool, bool> options = parse_options(argc, argv);
  Pacman pacman(options.first, options.second);
  pacman.run();
  exit(EXIT_SUCCESS);
}
