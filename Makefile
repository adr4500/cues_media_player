format :
	clang-format -i -style=file Src/*.cpp
	clang-format -i -style=file Inc/*.h
	cmake-format -i CMakeLists.txt
	cmake-format -i ThirdParty/CMakeLists.txt

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
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build