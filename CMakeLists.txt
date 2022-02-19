cmake_minimum_required(VERSION 3.16.3)
project(Spinners)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

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
	src/CStats.cpp
	src/CClickLocation.cpp
	src/CAnimation.cpp
	src/CWeapon.cpp
	src/Entity.cpp
	src/SUpdate.cpp
	src/SDraw.cpp
	src/Settings.cpp
	src/EntityManager.cpp
	src/Game.cpp
	src/Action.cpp
	src/ActionManager.cpp
	src/ActionStream.cpp
	src/Assets.cpp
	src/Animation.cpp
	src/AnimationSet.cpp
	src/Scene.cpp
	src/ScenePlay.cpp
	src/Interface.cpp
	src/Widget.cpp
	src/WidgetBox.cpp
	src/WidgetText.cpp
	src/Border.cpp
)

add_executable(Spinners ${SOURCE_FILES})
target_link_libraries(Spinners sfml-graphics sfml-audio)
include_directories(${PROJECT_SOURCE_DIR})
