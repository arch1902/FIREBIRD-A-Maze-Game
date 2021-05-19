## Installation Instructions 
Clone the repo onto your machine and then run the following command in the directory of the cloned repo:
* ```make``` To build the executable 
### For Single player 
Use ```./firebird-sdl server``` to play the game.
### For Multiplayer
* The first player will have to start the game using ```./firebird-sdl server``` and then select the two player option.
* The second player will have to run the command ```./firebird-sdl client``` and then select the two player option.  
On doing this the game would start for both the players.

## Libraries and Packages 
The following pakages and libraries were used in the project:
* sdl2          ```brew install sdl2```
* sdl2_mixer    ```brew install sdl2_mixer```
* sdl2_image   ```brew install sdl2_image```
* sdl2_net     ```brew install sdl2_net```
* sdl2_ttf     ```brew install sdl2_ttf```. 

The following come in Apple clang version 12.0.5
* socket.h
* netinet/in.h
* stdlib.h
* arpa/inet.h
* math.h
* ctime

              

## OS Details
Tested for macOS, Version 10.14 onwards

## Resources
* https://lazyfoo.net/tutorials/SDL/index.php
* https://galdin.dev/blog/getting-started-with-sdl/
* https://github.com/maciejspychala/sdl-game
* http://www.sdltutorials.com/sdl-net-part-1-user-tutorial
* https://www.makeschool.com/mediabook/oa/tutorials/trees-and-mazes/generating-a-maze-with-dfs/
* https://en.wikipedia.org/wiki/Maze-solving_algorithm
* https://github.com/schuay/pacman
* https://github.com/r6eve/pacman-sdl
* https://github.com/DarkMuffinJoe/Pacman
* https://thenumbat.github.io/cpp-course/sdl2/05/05.html
* https://www.educative.io/edpresso/how-to-implement-tcp-sockets-in-c
* https://docs.oracle.com/cd/E19683-01/816-5042/sockets-89916/index.html
