cmake_minimum_required(VERSION 3.16.3)
project(Spinners)
set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

set(SOURCE_FILES
	src/Spinners.cpp
	src/Vec2.cpp
	src/CTransform.cpp
	src/CCollision.cpp
	src/CShape.cpp
	src/CInput.cpp
	src/CLifespan.cpp
	src/CScore.cpp
	src/CTarget.cpp
	src/CCooldown.cpp
	src/Entity.cpp
	src/SLoad.cpp
	src/SUpdate.cpp
	src/SDraw.cpp
	src/SEnemySpawner.cpp
	src/Settings.cpp
	src/EntityManager.cpp
	src/Game.cpp
	src/Action.cpp
	src/ActionManager.cpp
	src/ActionStream.cpp
	src/ReplayManager.cpp
)

add_executable(Spinners ${SOURCE_FILES})
target_link_libraries(Spinners sfml-graphics sfml-audio)
include_directories(${PROJECT_SOURCE_DIR})
