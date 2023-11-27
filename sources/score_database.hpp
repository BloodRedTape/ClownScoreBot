#pragma once

#include <ini/ini.hpp>
#include <core/list.hpp>
#include <core/string.hpp>
#include <core/hash_table.hpp>

using ScoreList = List<int8_t>;
using ScoreMap = HashTable<String, ScoreList>;
using ChatMap = HashTable<int64_t, ScoreMap>;


class JsonDatabase {
	static constexpr const char *SectionName = "JsonDatabase";
private:
	ChatMap m_Map;
	String m_Filename;
public:
	JsonDatabase(const Ini &config);

	void Join(int64_t chat, const String &username);

	void Assign(int64_t chat, const String &username, int8_t score);

	int64_t Stats(int64_t chat, const String &username);

	HashTable<String, int64_t> Rating(int64_t chat);

	List<String> JoinedUsers(int64_t chat);

private:
	void SaveToFile();
};