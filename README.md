# cues_media_player
Pour les vidéos de Cues

Pour l'instant, pour l'utiliser, mettre dans le même dossier que l'executable un fichier Cues.csv et un fichier Video.mp4.
La vidéo mp4 doit absolument être encodée en video h264, audio mpeg, 25fps !

Le fichier CSV se rédige comme suit :

timecode,type,description
00:00:00:00,type,descr
...

Le timecode est formé de la façon suivante : heures:minutes:secondes:frame

### Types de timecode

Default : Apparait en blanc sur l'interface
GOTO : Invisible sur l'interface, sert de raccourci pour se déplacer dans la vidéo. Donnez leur un nom dans la description
Command : Execute une commande automatiquement quand on atteint le timecode. Entrer la commande dans la description
    - "Pause" met en pause le flux vidéo
    - "Goto ..." Permet d'aller automatiquement à un endroit (on peut mettre un timecode explicitement un le nom d'une cue GOTO)

## Getting started

Avant de pouvoir compiler le programme, vous devez installer FRUT. Pour cela, lancer les commandes suivantes dans un bash
```bash
cd ThirdParty/FRUT
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="../prefix" -DJUCE_ROOT="../../JUCE"
cmake --build . --target install
```

Pour pouvoir compiler le programme, il existe trois raccourcis :
```bash
make generate
```
Cette première commande fabrique le CMakeLists.txt (c'est FRUT qui s'en charge a partir du fichier cues_media_player.jucer)
```bash
make configure
make compile
```
Ces deux commandes configure puis compilent le projet CMake.

Pour éviter d'avoir à modifier a la main le fichier cues_media_player.jucer à la main, vous pouvez utiliser l'application dédiée Projucer.exe (indispensable à tout projet JUCE)
