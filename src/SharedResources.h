#ifndef SHARED_RESOURCES
#define SHARED_RESOURCES

#include "AppConfig.h"
#include "Assets.h"
#include "ActionManager.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>

extern AppConfig* app_conf;
extern Assets* assets;
extern ActionManager* act_mgr;
extern SoundManager* snd_mgr;

extern sf::RenderWindow* window;
extern sf::RenderTexture* screen_tex;
extern sf::Sprite* screen_sprite;
extern sf::View* game_view;

#endif
