format :
	clang-format -i -style=file Src/*.cpp
	cmake-format -i CMakeLists.txt

generate :
	ThirdParty/FRUT/prefix/FRUT/bin/Jucer2CMake reprojucer cues_media_player.jucer ThirdParty/FRUT/prefix/FRUT/cmake/Reprojucer.cmake
	sed -i '/include(Reprojucer)/a  \ \nif (MSVC)\nadd_compile_options(/bigobj)\nelse ()\nadd_compile_options(-Wa,-mbig-obj)\nendif ()' CmakeLists.txt
	cmake-format -i CMakeLists.txt

configure :
	cmake -S . -B build

compile :
	cmake --build build

clean :
	rm -rf build

run :
	./build/Debug/Cues_Media_Player.exe

build_release :
	rm -rf build
	ThirdParty/FRUT/prefix/FRUT/bin/Jucer2CMake reprojucer cues_media_player.jucer ThirdParty/FRUT/prefix/FRUT/cmake/Reprojucer.cmake
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release
