#include "SharedResources.h"

AppConfig* app_conf = nullptr;
Assets* assets = nullptr;
ActionManager* act_mgr = nullptr;
SoundManager* snd_mgr = nullptr;
ConfigManager* cfg_mgr = nullptr;
FrameManager* frame_mgr = nullptr;

sf::RenderWindow* window = nullptr;
sf::RenderTexture* screen_tex = nullptr;
sf::Sprite* screen_sprite = nullptr;

GameStats* game_stats;
