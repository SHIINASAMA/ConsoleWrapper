#pragma once
#include <map>
#include <string>
#include <memory>

class Config {
public:
	using ptr = std::shared_ptr<Config>;

	Config(std::map<std::wstring, std::wstring>* map);
	~Config();

	const wchar_t* get(const std::wstring& key);
	void put(const std::wstring& key, const std::wstring& value);
	bool emtpy();
	int size();

private:
	std::map<std::wstring, std::wstring>* attributes;
};