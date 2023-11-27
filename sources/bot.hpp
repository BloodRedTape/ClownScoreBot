#pragma once

#include <vector>
#include <memory>
#include <core/hash_table.hpp>
#include <core/string.hpp>
#include <tgbot/simple_bot.hpp>
#include <ini/ini.hpp>
#include <core/unique_ptr.hpp>
#include "score_database.hpp"

class ClownScoreBot;

struct UserState {
    virtual UniquePtr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query) = 0;
};

struct None : UserState {
    UniquePtr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

struct WaitForUsername : UserState {
    TgBot::Message::Ptr KeyboardMessage;
    String InitiatorUsername;

    WaitForUsername(TgBot::Message::Ptr keyboard_message, String initiator):
        KeyboardMessage(keyboard_message),
        InitiatorUsername(std::move(initiator))
    {}

    UniquePtr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

struct WaitForScore : UserState {
    TgBot::Message::Ptr KeyboardMessage;
    String InitiatorUsername;
    String TargetUsername;

    WaitForScore(TgBot::Message::Ptr keyboard_message, String initiator, String target):
        KeyboardMessage(keyboard_message),
        InitiatorUsername(std::move(initiator)),
        TargetUsername(std::move(target))
    {}
    
    UniquePtr<UserState> Update(ClownScoreBot &bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query)override;
};

class ClownScoreBot: public SimpleBot, public JsonDatabase{
public: 
    static constexpr const char *SectionName = "Bot";

    HashTable<String, UniquePtr<UserState>> m_Users;
public:
    ClownScoreBot(const Ini &config);

    void OnUnknownCommand(TgBot::Message::Ptr message) {
        UpdateUserState(message, nullptr);
    }

    void OnCallbackQuery(TgBot::CallbackQuery::Ptr query) {
        UpdateUserState(query->message, query);
    }

    void EnsureUserState(const String &username);

    void UpdateUserState(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query);

    static String GetUsername(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
        return String::FromStdString(query ? query->from->username : message->from->username);
    }

    void OnLog(String message);
};