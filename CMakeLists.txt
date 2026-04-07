cmake_minimum_required(VERSION 3.20)
project(ProtocolPhantom)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD ON)

add_executable(protocol_phantom
        src/main.cpp
        src/Item.cpp
        src/Enemy.cpp
        src/Puzzle.cpp
        src/Inventory.cpp
        src/Player.cpp
        src/Scene.cpp
        src/StoryScene.cpp
        src/CombatScene.cpp
        src/PuzzleScene.cpp
        src/ItemScene.cpp
        src/CSVLoader.cpp
        src/FileHandler.cpp
        src/GameManager.cpp
        src/InputValidator.cpp
        src/Display.cpp
        src/main.cpp
)
