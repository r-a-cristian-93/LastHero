#include "AppConfig.h"

AppConfig::AppConfig() {
	std::memset(scene_fade_frames, 20, sizeof(scene_fade_frames));
}

void AppConfig::read(std::string file_name) {
	std::ifstream file(file_name);
	std::string word;

	while(file >> word) {
		if (word == "WINDOW_NAME") {
			file >> window_name;
		}
		else if (word == "WINDOW_STYLE") {
			file >> word;
			if (word == "WINDOWED") {
				window_style = STYLE_WINDOWED;
			}
			else if (word == "FULLSCREEN") {
				window_style = STYLE_FULLSCREEN;
			}
			else {
				std::cout << "ERROR: invalid WINDOW_STYLE: " << word << std::endl;
			}
		}
		else if (word == "MAX_FPS") {
			file >> max_fps;
		}
		else if (word == "WINDOW_RES") {
			size_t mode_id = 0;
			file >> mode_id;
			if (mode_id < modes.size()) {
				current_mode_id = mode_id;
			}
			else {
				std::cout << "ERROR: Resolution not supported\n";
			}
		}
		else if (word == "MUSIC_VOLUME") {
			file >> music_volume;
		}
		else if (word == "SFX_VOLUME") {
			file >> sfx_volume;
		}
		else if (word == "GAME_RES") {
			file >> game_w;
			file >> game_h;
		}
		else if (word == "EDITOR_RES") {
			file >> editor_w;
			file >> editor_h;
		}
		else if (word == "GAME_SCALE") {
			file >> game_scale;
		}
		else if (word == "EDITOR_SCALE") {
			file >> editor_scale;
		}
		else if (word == "CAM_SPEED") {
			file >> cam_speed;
		}
		else if (word == "CAM_TRESHOLD") {
			file >> cam_treshold;
		}
		else if (word == "COLMAP_RES") {
			file >> colmap_res;
		}
		else if (word == "COLMAP_UPDATE") {
			file >> colmap_update;
		}
		else if (word == "FADE_SCENE") {
			int frames = 1;

			for (size_t f_type=1; f_type<FADE::COUNT; f_type++) {
				file >> frames;
				if (frames <= 0) {
					frames = 1;
					std::cout << "FADE_SCENE frames must be greater than 0. Set to default value 1.\n";
				}
				if (frames > 255) {
					frames = 255;
					std::cout << "FADE_SCENE frames must be lower than 256. Set to default value 1.\n";
				}

				scene_fade_frames[f_type] = frames;
			}
		}
		else if (word == "FRAMES_SCORE") {
			int frames = 1;

			for (size_t f_type=1; f_type<FRAMES_SCORE::COUNT; f_type++) {
				file >> frames;
				if (frames <= 0) {
					frames = 1;
					std::cout << "FRAMES_SCORE frames must be greater than 0. Set to default value 1.\n";
				}

				score_key_frames[f_type] = frames;
			}
		}
		else if (word == "FADE_MULTIPLYER") {
			float m = 1;
			file >> m;

			if (m <= 0) {
				m = 1;
				std::cout << "Fade multipilier must be greater than 0. Set to default value 1.\n";
			}
			fade_multiplier = m;

			// apply multiplier
			for (size_t f=0; f<FADE::COUNT; f++) {
				scene_fade_frames[f] *= m;
			}
			for (size_t f=0; f<FRAMES_SCORE::COUNT; f++) {
				score_key_frames[f] *= m;
			}
		}
	}

	file.close();
}

void AppConfig::write(std::string file_name) {
	std::ofstream file(file_name);

	file << "// WINDOWED or FULLSCREEN.\n";
	file << "WINDOW_STYLE ";
	if (window_style == STYLE_WINDOWED) {
		file << "WINDOWED\n\n";
	}
	else if (window_style == STYLE_FULLSCREEN) {
		file << "FULLSCREEN\n\n";
	}

	file << "// Window resolution. 0 to maximum number of available resolutions.\n";
	file << "WINDOW_RES " << current_mode_id << "\n\n";

	file << "// Background music volume 0-100.\n";
	file << "MUSIC_VOLUME " << music_volume << "\n\n";

	file << "// Sound effects volume 0-100.\n";
	file << "SFX_VOLUME " << sfx_volume;
}
