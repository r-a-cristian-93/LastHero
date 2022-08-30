#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

#include "AppConfig.h"

class ConfigManager {
public:
	void applySettings(AppConfig& conf);
};

#endif
