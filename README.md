# DonkeyKong
Gameboy Advance Donkey Kong Game

## To Run

Installs ​ the​ ​ GBA​ ​ emulator​ ​ and​ ​ the​ ​ compiler​ ​ toolchain.</br>
```sudo​ apt-get​ install​ gcc-arm-none-eabi```</br>
```sudo​ apt-get​ install​ libnewlib-arm-none-eabi```</br>
```sudo​ apt-get​ install​ cs2110-vbam-sdl```</br>

Clone this directory.</br>
```git clone https://github.com/tomonarifeehan/DonkeyKong.git```</br>

Change into directory.</br>
```cd DonkeyKong```</br>

Removes all compiled files.</br>
```make clean```</br>

Runs Gameboy Emulator.</br>
```make vba```</br>

## Game Instructions:
The goal of this game is to leap over the barrels, climb the ladders, and to reach Donkey Kong and save the princess.</br>

<strong>Keys:</strong></br>
Z: Jump</br>
X: Pause</br>
Backspace: Return to Splash Screen (works on every screen)</br>
Start: Start Game/Resume Game</br></br>

To see win screen: Collide with Donky Kong.</br>
To see lose screen: Collide with a barrel.</br></br>

![Alt Text](https://github.com/tomonarifeehan/DonkeyKong/blob/master/img/final_game.gif)