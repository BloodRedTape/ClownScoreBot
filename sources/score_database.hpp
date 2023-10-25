#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <INIReader.h>

using ScoreList = std::vector<int8_t>;
using ScoreMap = std::unordered_map<std::string, ScoreList>;
using ChatMap = std::unordered_map<int64_t, ScoreMap>;


class JsonDatabase {
	static constexpr const char *SectionName = "JsonDatabase";
private:
	ChatMap m_Map;
	std::string m_Filename;
public:
	JsonDatabase(const INIReader &config);

	void Join(int64_t chat, const std::string &username);

	void Assign(int64_t chat, const std::string &username, int8_t score);

	int64_t Stats(int64_t chat, const std::string &username);

	std::unordered_map<std::string, int64_t> Rating(int64_t chat);

	std::vector<std::string> JoinedUsers(int64_t chat);

private:
	void SaveToFile();
};