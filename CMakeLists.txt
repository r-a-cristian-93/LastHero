cmake_minimum_required(VERSION 3.16.3)
project(LastHero)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

set(SOURCE_FILES
	src/SharedResources.cpp
	src/LastHero.cpp
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
	src/CFx.cpp
	src/CSfx.cpp
	src/CBChase.cpp
	src/CBFire.cpp
	src/CBPatrol.cpp
	src/CBPowerup.cpp
	src/Entity.cpp
	src/SUpdate.cpp
	src/SDraw.cpp
	src/Settings.cpp
	src/EntityManager.cpp
	src/Game.cpp
	src/Action.cpp
	src/ActionManager.cpp
	src/Assets.cpp
	src/Animation.cpp
	src/AnimationSet.cpp
	src/Scene.cpp
	src/ScenePlay.cpp
	src/SceneMainMenu.cpp
	src/SceneSettings.cpp
	src/SceneGameOver.cpp
	src/SceneScore.cpp
	src/SceneEditor.cpp
	src/Interface.cpp
	src/Widget.cpp
	src/Border.cpp
	src/Tilemap.cpp
	src/Level.cpp
	src/SoundManager.cpp
	src/AStarContainer.cpp
	src/AStarNode.cpp
	src/MapCollision.cpp
	src/AppConfig.cpp
	src/ConfigManager.cpp
)

add_executable(LastHero ${SOURCE_FILES})
target_link_libraries(LastHero sfml-graphics sfml-audio)
include_directories(${PROJECT_SOURCE_DIR})
