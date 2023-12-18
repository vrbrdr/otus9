cmake_minimum_required(VERSION 3.16)
project(snake LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-g -Wall -Wextra -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/EHsc /W4 /WX)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(COMMON_SRCS
controllers/EmptyController.hpp
controllers/KeyboardPlayerController.hpp
controllers/PlayerController.hpp
controllers/RemoteController.hpp
controllers/RemotePlayerController.hpp
controllers/RemoteServerController.hpp

device/Device.hpp
device/DrawDevice.hpp
device/InputDevice.hpp

network/Message.hpp
network/Network.hpp

sfml/SfmlDevice.hpp
sfml/SfmlDrawDevice.cpp
sfml/SfmlDrawDevice.hpp
sfml/SfmlInputDevice.cpp
sfml/SfmlInputDevice.hpp
sfml/SfmlNetExchange.cpp
sfml/SfmlNetExchange.hpp
sfml/SfmlTcpGameClient.hpp
sfml/SfmlTcpGameServer.hpp

Application.cpp
Application.hpp
Game.cpp
Game.hpp
GameState.hpp
Player.cpp
Player.hpp
)

add_subdirectory(tests)
add_executable(snake main.cpp ${COMMON_SRCS})

target_link_libraries(snake 
    PRIVATE 
    sfml-graphics
    sfml-network
)

target_compile_features(snake PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET snake
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:snake>
        VERBATIM)
endif()

install(TARGETS snake)
