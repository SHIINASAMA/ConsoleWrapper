#include "ConfigHelper.h"
#include <fstream>
#include <memory>

Config::ptr ConfigHelper::BuildConfig()
{
	auto map = new std::map<std::wstring, std::wstring>();
	std::wifstream stream;
	stream.open("config.ini", std::ios::in);
	if (stream.good()) {
		wchar_t buffer[1024]{0};
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
				wchar_t* keyBuffer = nullptr;
				wchar_t* valueBuffer = nullptr;
				//memcpy_s(keyBuffer, 1024, buffer, spilt);
				//memcpy_s(valueBuffer, 1024, buffer + spilt + 1, length - spilt - 1);
				keyBuffer = wcstok(buffer, L"=", &valueBuffer);
				map->emplace(std::wstring{ keyBuffer }, std::wstring{ valueBuffer });
			}
		}
		stream.close();
	}
	else {
		printf_s("%s\n", strerror(errno));
	}
	return std::make_shared<Config>(map);
}