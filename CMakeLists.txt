cmake_minimum_required(VERSION 2.6)
project(Asteroids)

SET(CMAKE_CXX_FLAGS_RELEASE "/MDd")

include_directories(3rdParty/RakNet/Source)

find_library(RAKNET_LIB
NAMES RakNetLibStatic
HINTS ${CMAKE_SOURCE_DIR}/3rdParty/RakNet)

include_directories(${CMAKE_SOURCE_DIR}/3rdParty/SFML/include)
link_directories(${CMAKE_SOURCE_DIR}/3rdParty/SFML)

#add_library(RenderGame src/renderGame.h src/renderGame.cpp)

set(GAMEFILES src/Asteroid.cpp src/Asteroid.h src/Bullet.h src/Bullet.cpp src/Ball.cpp  src/Ball.h  src/Block.cpp  src/Block.h  src/Game.cpp  src/Game.h  src/GameObject.cpp  src/GameObject.h  src/Paddle.cpp  src/Paddle.h 
src/Common/InterpolatedPosition.cpp src/Common/InterpolatedPosition.h)

set(NETWORKFILES src/Client/Client.h src/Client/Client.cpp src/Common/Common.h src/Common/Common.cpp)

add_executable(Server src/Server/Server.cpp src/Server/Game.h src/Server/Game.cpp src/Server/Player.h src/Server/Player.cpp src/Common/Common.h src/Common/Common.cpp)
add_executable(Game src/Main.cpp ${GAMEFILES} ${NETWORKFILES})

target_link_libraries(Server sfml-graphics sfml-system sfml-window)
target_link_libraries(Game sfml-graphics sfml-system sfml-window sfml-audio)

set(DEP_LIBS
	sfml-system
	sfml-graphics
	sfml-window
	sfml-audio
	RakNetLibStatic
)

target_link_libraries(Server ${RAKNET_LIB} ws2_32)
target_link_libraries(Game ${RAKNET_LIB} ws2_32)

add_custom_command(TARGET Server PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:Server>)
					   
add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/3rdParty/SFML/bin"
        "${PROJECT_BINARY_DIR}/Debug")

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/3rdParty/SFML/bin"
        "${PROJECT_BINARY_DIR}/Release")