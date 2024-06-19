#include "score_database.hpp"
#include "utils.hpp"
#include <core/os/file.hpp>
#include <json/json.hpp>
#include <ini/ini.hpp>
#include <core/assert.hpp>

JsonDatabase::JsonDatabase(const Ini& config):
	m_Filename(
		config.GetString(SectionName, "Filepath")
	)
{
	auto content = File::ReadEntire(m_Filename);

	if(content.HasValue()){
		m_Map = Json::parse(content.Value());
	}else {
		SX_ASSERT(false, "Can't read file for some reason");
	}
}

void JsonDatabase::Join(int64_t chat, const String& username){
	m_Map[chat][username];

	SaveToFile();
}

void JsonDatabase::Assign(int64_t chat, const String& username, int8_t score){
	m_Map[chat][username].Add(score);

	SaveToFile();
}

int64_t JsonDatabase::Stats(int64_t chat, const String& username){
	int64_t score = 0;

	for (int8_t one : m_Map[chat][username]) {
		score += one;
	}

	return score;
}

int64_t JsonDatabase::Stats(const String& username) {
	int64_t score = 0;
	
	for (auto [chat, _] : m_Map) {
		score += Stats(chat, username);
	}

	return score;
}

HashTable<String, int64_t> JsonDatabase::Rating(int64_t chat) {
	HashTable<String, int64_t> rating;

	for (const auto& [username, _] : m_Map[chat]) {
		rating[username] = Stats(chat, username);
	}

	return rating;
}

List<String> JsonDatabase::JoinedUsers(int64_t chat) {
	return m_Map[chat].Keys();
}

void JsonDatabase::SaveToFile()
{
	File::WriteEntire(m_Filename, Json(m_Map).dump(1, '\t'));
}
