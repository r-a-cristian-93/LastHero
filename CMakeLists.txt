cmake_minimum_required(VERSION 3.16.3)
project(LastHero)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

set(SOURCE_COMMON
	src/SharedResources.cpp
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
	src/EntityManager.cpp
	src/Action.cpp
	src/ActionManager.cpp
	src/Assets.cpp
	src/Animation.cpp
	src/AnimationSet.cpp
	src/Scene.cpp
	src/ScenePlay.cpp
	src/Interface.cpp
	src/Widget.cpp
	src/Box.cpp
	src/Tilemap.cpp
	src/Level.cpp
	src/SoundManager.cpp
	src/AStarContainer.cpp
	src/AStarNode.cpp
	src/MapCollision.cpp
	src/AppConfig.cpp
	src/ConfigManager.cpp
	src/GameStats.cpp
	src/App.cpp
	src/Link.cpp
	src/WCText.cpp
	src/WCBox.cpp
	src/WCImage.cpp
)

set (SOURCE_GAME
	src/LastHero.cpp
	src/Game.cpp
	src/SceneMainMenu.cpp
	src/SceneSettings.cpp
	src/SceneGameOver.cpp
	src/SceneScore.cpp
)

add_library(common SHARED ${SOURCE_COMMON})
target_link_libraries(common sfml-graphics sfml-audio)


add_executable(LastHero ${SOURCE_GAME})
target_link_libraries(LastHero sfml-graphics sfml-audio common)

include_directories(${PROJECT_SOURCE_DIR})
