#include "util.h"

std::shared_ptr<spdlog::logger> gLogger;

bool Util::showTitleScreen(int argc, char *argv[]) {
	bool showTitle = true;
	if(argc > 1) {
		for (int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-h") == 0) {
				showTitle = false;
			}
		}
		
	}
	return showTitle;
}

void Util::initLogger(int argc, char *argv[]) {
	try {
		gLogger = spdlog::basic_logger_mt("CApp", "logs/CApp.log");
		spdlog::set_default_logger(gLogger);
		LOG_LEVEL level = DEBUG;
		if(argc > 1) {
			for (int i = 0; i < argc; i++) {
				switch (strcmp(argv[i], "-l")) {
					case 0:
						if(i + 1 < argc) {
							if(strcmp(argv[i + 1], "trace") == 0) {
								level = TRACE;
								gLogger->info("Setting log level to TRACE");
							}
							else if(strcmp(argv[i + 1], "debug") == 0) {
								level = DEBUG;
								gLogger->info("Setting log level to DEBUG");
							}
							else if(strcmp(argv[i + 1], "info") == 0) {
								level = INFO;
								gLogger->info("Setting log level to INFO");
							}
							else if(strcmp(argv[i + 1], "warn") == 0) {
								level = WARN;
								gLogger->info("Setting log level to WARN");
							}
							else if(strcmp(argv[i + 1], "err") == 0) {
								level = ERR;
								gLogger->info("Setting log level to ERR");
							}
							else if(strcmp(argv[i + 1], "critical") == 0) {
								level = CRITICAL;
								gLogger->info("Setting log level to CRITICAL");
							}
						}
						break;
				}
			}
		}
		switch (level) {
			case TRACE:
				spdlog::set_level(spdlog::level::trace);
				break;
			case DEBUG:
				spdlog::set_level(spdlog::level::debug);
				break;
			case INFO:
				spdlog::set_level(spdlog::level::info);
				break;
			case WARN:
				spdlog::set_level(spdlog::level::warn);
				break;
			case ERR:
				spdlog::set_level(spdlog::level::err);
				break;
			case CRITICAL:
				spdlog::set_level(spdlog::level::critical);
				break;
			default:
				spdlog::set_level(spdlog::level::debug);
				break;
		}
		spdlog::set_level(spdlog::level::debug);
		spdlog::flush_every(std::chrono::milliseconds(5));
	}
	catch(const spdlog::spdlog_ex& ex) {
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}
}

std::vector<int> Util::readSettingsFromFile(const char* path) {
    std::ifstream settings;
    settings.open(path, std::ios::in);
    if(!settings.is_open()) {
        spdlog::error("Unable to open file! {}", path);
        return std::vector<int>();
    }
    std::vector<int> values;
    std::string line;
    while(std::getline(settings, line)) {
        std::stringstream ss(line);
        int a;
        if(ss >> a) {
            values.push_back(a);
        }
    }
    settings.close();
    return values;
}

void Util::saveSettingsToFile(const char* path, int values[]) {
    std::ofstream settings;
    settings.open(path, std::ios::trunc);
    if(!settings.is_open()) {
        spdlog::error("Unable to open file! {}", path);
        return;
    }
    for(int i(0); i < LEFT_MENU; i++) {
        int a = values[i];
        std::stringstream ss;
        ss << a;
        std::string str = ss.str();
        settings << str + "\n";
    }
    settings.close();
}

void Util::flushEvents() {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP);
}