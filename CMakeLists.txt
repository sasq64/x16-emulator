project(x16-emu)
cmake_minimum_required(VERSION 3.10)

find_package(SDL2 REQUIRED)

set(SOURCES cpu/fake6502.c memory.c disasm.c video.c ps2.c via.c loadsave.c
	spi.c vera_spi.c audio.c vera_pcm.c vera_psg.c sdcard.c main.c debugger.c
	javascript_interface.c joystick.c rendertext.c keyboard.c icon.c
	extern/src/ym2151.c)

SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES LANGUAGE CXX)

add_executable(x16-emu ${SOURCES})
target_compile_options(x16-emu PRIVATE -Wall -std=c++17)
target_include_directories(x16-emu PRIVATE extern/include extern/src ${SDL2_INCLUDE_DIRS})
target_link_libraries(x16-emu PRIVATE m ${SDL2_LIBRARIES})
