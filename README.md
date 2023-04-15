# cues_media_player
Pour les vidéos de Cues


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
