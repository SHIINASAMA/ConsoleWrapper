#include "Config.h"

Config::Config(std::map<std::string, std::string>* map) {
	this->attributes = map;
}

Config::~Config() {
	if (attributes) {
		delete attributes;
	}
}

const char* Config::get(const std::string& key) {
	auto value = attributes->find(key);
	if (value == attributes->end()) {
		return nullptr;
	}
	else {
		return value->second.c_str();
	}
}

void Config::put(const std::string& key, const std::string& value) {
	attributes->emplace(key, value);
}

bool Config::emtpy()
{
	return attributes->empty();
}

int Config::size()
{
	return attributes->size();
}
