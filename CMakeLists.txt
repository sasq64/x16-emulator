cmake_minimum_required(VERSION 3.5)
project(x16-emu VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_subdirectory(external/lua)
add_subdirectory(external/sol3)

set(SOURCES cpu/fake6502.c memory.c disasm.c video.c ps2.c via.c loadsave.c
	spi.c vera_spi.c audio.c vera_pcm.c vera_psg.c sdcard.c main.c debugger.c
	javascript_interface.c joystick.c rendertext.c keyboard.c icon.c
	extern/src/ym2151.c script.cpp)

SET_SOURCE_FILES_PROPERTIES(${SOURCES} PROPERTIES LANGUAGE CXX)

add_executable(x16-emu ${SOURCES})
target_compile_options(x16-emu PRIVATE -Wall)
target_compile_definitions(x16-emu PRIVATE CPP_OPT SOL_USING_CXX_LUA )
target_include_directories(x16-emu PRIVATE extern/include extern/src ${SDL2_INCLUDE_DIRS})
target_link_libraries(x16-emu PRIVATE lua sol2::sol2 m ${SDL2_LIBRARIES})
