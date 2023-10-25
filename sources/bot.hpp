#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "simple_bot.hpp"
#include "INIReader.h"
#include "score_database.hpp"

class ClownScoreBot;

struct UserState {
    virtual std::unique_ptr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query) = 0;
};

struct None : UserState {
    std::unique_ptr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

struct WaitForUsername : UserState {
    TgBot::Message::Ptr KeyboardMessage;

    WaitForUsername(TgBot::Message::Ptr keyboard_message):
        KeyboardMessage(keyboard_message)
    {}

    std::unique_ptr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

struct WaitForScore : UserState {
    TgBot::Message::Ptr KeyboardMessage;
    std::string Username;

    WaitForScore(TgBot::Message::Ptr keyboard_message, std::string username):
        KeyboardMessage(keyboard_message),
        Username(std::move(username))
    {}
    
    std::unique_ptr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

class ClownScoreBot: public SimpleBot, public JsonDatabase{
public: 
    static constexpr const char *SectionName = "Bot";

    std::unordered_map<std::string, std::unique_ptr<UserState>> m_Users;
public:
    ClownScoreBot(const INIReader &config);

    void OnUnknownCommand(TgBot::Message::Ptr message) {
        UpdateUserState(message, nullptr);
    }

    void OnCallbackQuery(TgBot::CallbackQuery::Ptr query) {
        UpdateUserState(query->message, query);
    }

    void EnsureUserState(const std::string &username);

    void UpdateUserState(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query);

    static std::string GetUsername(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
        return query ? query->from->username : message->from->username;
    }

    void OnLog(std::string message);
};