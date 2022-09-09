# LastHero

LastHero is a very customizable single-player 2D shooter game that allows users to create new game entities and levels.

## Development and Release

The game is still under development and new features will be added:
- Choose between multiple heroes.
- More game level and mechanics.
- A fully functional level editor.

## Gameplay

The main goal is to kill all the enemies on the map and at the same time to protect you only resource left after the alien invation, your precious holstein cow.
Throughout the map you can find diverse powerups to help you in your battle with the vicious aliens.

## How to compile on Windows

Download and install mingw32 SFML and make.
```
http://gnuwin32.sourceforge.net/downlinks/make-dep-zip.php
http://gnuwin32.sourceforge.net/downlinks/make-bin-zip.php
https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download
https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit.zip
```

Download the source code of the game.
```
git clone git@github.com:r-a-cristian-93/LastHero.git
```

Navigate inside the game directory you downloaded and build the game.

```
cmake -G "MinGW Makefiles" .
make
```

## Screenshots

<img src="/demo/s01.png" />
<img src="/demo/s02.png" />
<img src="/demo/s03.png" />
<img src="/demo/s04.png" />
