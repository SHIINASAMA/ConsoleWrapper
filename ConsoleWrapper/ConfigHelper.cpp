#include "ConfigHelper.h"
#include <fstream>
#include <memory>

Config::ptr ConfigHelper::BuildConfig()
{
	auto map = new std::map<std::string, std::string>();
	std::ifstream stream;
	stream.open("config.ini", std::ios::in);
	if (stream.good()) {
		char buffer[1024]{0};
		while (!stream.eof()) {
			stream.getline(buffer, 1024);
			int length = stream.gcount();
			int spilt = -1;
			for (int i = 0; i < length; i++) {
				if (buffer[i] == '=') {
					spilt = i;
					break;
				}
			}

			if (spilt != -1) {
				char keyBuffer[1024]{0};
				char valueBuffer[1024]{ 0 };
				memcpy_s(keyBuffer, 1024, buffer, spilt);
				memcpy_s(valueBuffer, 1024, buffer + spilt + 1, length - spilt - 1);
				map->emplace(std::string{ keyBuffer }, std::string{ valueBuffer });
			}
		}
		stream.close();
	}
	return std::make_shared<Config>(map);
}