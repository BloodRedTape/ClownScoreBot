#include "score_database.hpp"
#include "utils.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

JsonDatabase::JsonDatabase(const INIReader& config):
	m_Filename(
		config.GetString(SectionName, "Filepath", "")
	)
{
	m_Map = json::parse(ReadEntireFile(m_Filename));
}

void JsonDatabase::Join(int64_t chat, const std::string& username){
	m_Map[chat][username];

	SaveToFile();
}

void JsonDatabase::Assign(int64_t chat, const std::string& username, int8_t score){
	m_Map[chat][username].push_back(score);

	SaveToFile();
}

int64_t JsonDatabase::Stats(int64_t chat, const std::string& username){
	int64_t score = 0;

	for (int8_t one : m_Map[chat][username]) {
		score += one;
	}

	return score;
}

std::unordered_map<std::string, int64_t> JsonDatabase::Rating(int64_t chat) {
	std::unordered_map<std::string, int64_t> rating;

	for (const auto& [username, _] : m_Map[chat]) {
		rating[username] = Stats(chat, username);
	}

	return rating;
}

std::vector<std::string> JsonDatabase::JoinedUsers(int64_t chat) {
	return Keys(m_Map[chat]);
}

void JsonDatabase::SaveToFile()
{
	WriteEntireFile(m_Filename, json(m_Map).dump(1, '\t'));
}
