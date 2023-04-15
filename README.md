# cues_media_player
Pour les vidéos de Cues
Pour l'instant, pour l'utiliser, mettre dans le même dossier que l'executable un fichier Cues.csv et un fichier Video.mp4.

Le fichier CSV se rédige comme suit :

timecode,nom,description
00:00:00:00,name,descr
...

Le timecode est formé de la façon suivante : heures:minutes:secondes:frame

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
