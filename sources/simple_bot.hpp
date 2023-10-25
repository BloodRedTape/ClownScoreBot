#pragma once

#include <string>
#include <vector>
#include <tgbot/Bot.h>
#include "utils.hpp"
#include "format.hpp"

struct KeyboardButton {
    std::string Text;
    std::string CallbackData;

    KeyboardButton(std::string text, std::string callback_data) :
        Text(std::move(text)),
        CallbackData(std::move(callback_data))
    {}

    KeyboardButton(std::string text) :
        Text(text),
        CallbackData(text)
    {}
};

using KeyboardLayout = std::vector<std::vector<KeyboardButton>>;

namespace Keyboard {
    KeyboardLayout ToKeyboard(const std::vector<std::string> &texts);
}

class SimpleBot: private TgBot::Bot {
    std::function<void(std::string)> m_OnLog;
public:
    SimpleBot(const std::string& token);

    int Run();

    TgBot::Message::Ptr SendMessage(const TgBot::Message::Ptr& source, const std::string& message);

    TgBot::Message::Ptr SendMessage(const TgBot::Message::Ptr& source, const std::string& message, TgBot::GenericReply::Ptr reply);

    TgBot::Message::Ptr SendKeyboard(const TgBot::Message::Ptr& source, const std::string& message, const KeyboardLayout& keyboard);

    void EditMessage(TgBot::Message::Ptr message, const std::string &text, TgBot::GenericReply::Ptr reply);

    void EditMessage(TgBot::Message::Ptr message, const std::string &text, const KeyboardLayout &keyboard);

    void EditMessage(TgBot::Message::Ptr message, const KeyboardLayout &keyboard);

    void EditMessage(TgBot::Message::Ptr message, const std::string &text);

    void DeleteMessage(TgBot::Message::Ptr message);

    bool AnswerCallbackQuery(const std::string& callbackQueryId, const std::string& text = "");
    
    template<typename...ArgsType>
    void Log(const char *fmt, const ArgsType&...args) {
        m_OnLog(Format(fmt, args...));
    }

    template<typename Class>
    void BindOnMessage(Class* object, void (Class::* method)(TgBot::Message::Ptr)) {
        getEvents().onUnknownCommand(std::bind(method, object, std::placeholders::_1));
    }

    template<typename Class>
    void BindOnUnknownCommand(Class* object, void (Class::* method)(TgBot::Message::Ptr)) {
        getEvents().onUnknownCommand(std::bind(method, object, std::placeholders::_1));
    }

    template<typename Class>
    void BindOnCommand(const std::string &command, Class* object, void (Class::* method)(TgBot::Message::Ptr)) {
        getEvents().onCommand(command, std::bind(method, object, std::placeholders::_1));
    }

    template<typename Class>
    void BindOnCommand(std::initializer_list<std::string> commands, Class* object, void (Class::* method)(TgBot::Message::Ptr)) {
        getEvents().onCommand(commands, std::bind(method, object, std::placeholders::_1));
    }

    template<typename Class>
    void BindOnCallbackQuery(Class* object, void (Class::* method)(TgBot::CallbackQuery::Ptr)) {
        getEvents().onCallbackQuery(std::bind(method, object, std::placeholders::_1));
    }

    template<typename Class>
    void BindOnLog(Class* object, void (Class::* method)(std::string)) {
        m_OnLog= std::bind(method, object, std::placeholders::_1);
    }
};