#include "Config.h"

Config::Config(std::map<std::wstring, std::wstring>* map) {
	this->attributes = map;
}

Config::~Config() {
	if (attributes) {
		delete attributes;
	}
}

const wchar_t* Config::get(const std::wstring& key) {
	auto value = attributes->find(key);
	if (value == attributes->end()) {
		return nullptr;
	}
	else {
		return value->second.c_str();
	}
}

void Config::put(const std::wstring& key, const std::wstring& value) {
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
