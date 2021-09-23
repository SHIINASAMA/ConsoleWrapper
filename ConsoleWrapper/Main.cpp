#include "ConfigHelper.h"
#include <iostream>

int main(int argc, char** argv) {
	std::cout << argv[0] << std::endl;
	auto config = ConfigHelper::BuildConfig();
	auto cmd = config->get("cmd");
	auto icon = config->get("icon");
	return 0;
}