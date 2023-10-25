#include "simple_bot.hpp"
#include <tgbot/net/TgLongPoll.h>

KeyboardLayout Keyboard::ToKeyboard(const std::vector<std::string>& texts) {
    KeyboardLayout layout;
    std::vector<KeyboardButton> row;

    for (const auto &text : texts) {
        row.emplace_back(text);
    }

    layout.push_back(std::move(row));

    return layout;
}

SimpleBot::SimpleBot(const std::string& token) :
    TgBot::Bot(token)
{}

int SimpleBot::Run() {
    Log("Started");

    try {
        TgBot::TgLongPoll long_poll(*this);

        while (true) {
            long_poll.start();
        }

    }
    catch (const std::exception& exception) {
        Log("Crash: %", exception.what());
    }

    return EXIT_SUCCESS;
}

TgBot::Message::Ptr SimpleBot::SendMessage(const TgBot::Message::Ptr& source, const std::string& message) {
    return SendMessage(source, message, nullptr);
}

TgBot::Message::Ptr SimpleBot::SendMessage(const TgBot::Message::Ptr& source, const std::string& message, TgBot::GenericReply::Ptr reply) {
    if (!message.size()) {
        Log("Can't send empty messages");
        return nullptr;
    }

    TgBot::Message::Ptr result = nullptr;

    try {
        result = getApi().sendMessage(source->chat->id, message, false, 0, reply, "", false, {}, false, false, source->isTopicMessage ? source->messageThreadId : 0);
    }
    catch (const std::exception& exception) {
        auto chat = source->chat;

        std::string chat_name = chat->username.size() ? chat->username : chat->title;

        Log("Failed to send message to chat '%' id % reason %", chat_name, chat->id, exception.what());
    }

    return result;
}

TgBot::Message::Ptr SimpleBot::SendKeyboard(const TgBot::Message::Ptr& source, const std::string& message, const KeyboardLayout& keyboard) {
    using namespace TgBot;

    InlineKeyboardMarkup::Ptr keyboard_markup(new InlineKeyboardMarkup);

    for (const auto& row : keyboard) {
        std::vector<InlineKeyboardButton::Ptr> row_markup;

        for (const auto& button : row) {

            InlineKeyboardButton::Ptr button_markup(new InlineKeyboardButton);
            button_markup->text = button.Text;
            button_markup->callbackData = button.CallbackData;

            row_markup.push_back(button_markup);
        }

        keyboard_markup->inlineKeyboard.push_back(row_markup);
    }

    return SendMessage(source, message, keyboard_markup);
}

bool SimpleBot::AnswerCallbackQuery(const std::string& callbackQueryId, const std::string& text) {
    
    try{
        return getApi().answerCallbackQuery(callbackQueryId, text);
    }
    catch (const std::exception& exception) {
        Log("Failed to answer callback query %", callbackQueryId);
    }
    return false;
}

void SimpleBot::DeleteMessage(TgBot::Message::Ptr message) {
    try{
        getApi().deleteMessage(message->chat->id, message->messageId);
    }
    catch (const std::exception& exception) {
        auto chat = message->chat;

        std::string chat_name = chat->username.size() ? chat->username : chat->title;

        Log("Failed to delete message % from chat %, id %, reason: %", message->messageId, chat_name, chat->id, exception.what());
    }
}
