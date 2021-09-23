#pragma once
#include <map>
#include <string>
#include <memory>

class Config {
public:
	using ptr = std::shared_ptr<Config>;

	Config(std::map<std::string, std::string>* map);
	~Config();

	const char* get(const std::string& key);
	void put(const std::string& key, const std::string& value);
	bool emtpy();
	int size();

private:
	std::map<std::string, std::string>* attributes;
};