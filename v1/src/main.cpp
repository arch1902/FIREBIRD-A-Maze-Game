#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include "pacman.hpp"

const char Version[] = "v0.3.1";

std::pair<bool, bool> parse_options(const int argc, char **argv)  {
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
        std::cout << R"(Usage: pacman-sdl [options]

Options:
    -f  --fullscreen    fullscreen mode
    -h, --help          print this help menu
    -v, --version       print version
)";
        exit(EXIT_SUCCESS);
      case 'f':
        fullscreen_mode = true;
        break;
      case 'v':
        std::cout << Version << '\n';
        exit(EXIT_SUCCESS);
      case '?': {
        std::string av(argv[curind]);
        int n = 0;
        while (av[n] == '-') {
          ++n;
        }
        av.erase(av.begin(), av.begin() + n);
        std::cerr << "Unrecognized option: '" << av << "'\n";
        exit(EXIT_FAILURE);
      }
      default:
        // do nothing
        break;
    }
  }

  return std::make_pair(fullscreen_mode, false);
}

int main(int argc, char **argv) {
  const std::pair<bool, bool> opts = parse_options(argc, argv);
  Pacman pacman(opts.first, opts.second);
  pacman.run();
  exit(EXIT_SUCCESS);
}