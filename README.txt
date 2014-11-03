
***Sve library-e uzmite 32 bitne, i u Cmake-u buildajte 32 bitni projekt***

Sta treba instalirat:

- C++11 compiler i IDE (preporucam Visual Studio 2012 ili 2013)
- SDL2 library: https://www.libsdl.org/download-2.0.php
- glew library: http://glew.sourceforge.net/
- OpenGL 3.3+ (ovo vjerojatno imate)
- Cmake: http://www.cmake.org/

Kako buildat:

1. Instalirate sve ovo gore
2. Napravite environment varijablu SDL2DIR koja pokazuje na root SDL2 direktorija
2. Napravite environment varijablu GLEW_HOME koja pokazuje na root GLEW direktorija
4. Stavite put do glew32.dll i sdl2.dll u PATH (ili ih metnete u system folder od windowsa)
3. Pokrenete CMake gui i u "Where is the source code" metnete path do ovog direktorija.
   U "Where to build the binaries" metnete gdje vec ocete da se napravi projekt.
   Zatim stisnete dole Configure, pa vas pite za koj IDE i kompajler da napravi projekt, odaberite sto hocete/imate.
   Zatim stisnete Generate, i kad zavrsi gotovi ste.
4. Pokrenite projekt i buildajte!

Ako ima problema vicite.
