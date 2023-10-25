#include "bot.hpp"
#include <tgbot/net/TgLongPoll.h>
#include <exception>
#include <string_view>
#include "utils.hpp"
#include "format.hpp"

ClownScoreBot::ClownScoreBot(const INIReader &config):
    SimpleBot(
        config.GetString(SectionName, "Token", "")
    ),
    JsonDatabase(
        config
    )
{
    BindOnUnknownCommand(this, &ClownScoreBot::OnUnknownCommand);
    BindOnCallbackQuery(this, &ClownScoreBot::OnCallbackQuery);
    BindOnLog(this, &ClownScoreBot::OnLog);
}

void ClownScoreBot::UpdateUserState(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    const std::string &username = query ? query->from->username : message->from->username;

    EnsureUserState(username);

    auto &state = m_Users[username];

    auto new_state = state->Update(*this, message, query);

    if (new_state){
        state = std::move(new_state);
    }
}

void ClownScoreBot::EnsureUserState(const std::string& username) {
    if(!m_Users.count(username))
        m_Users[username] = std::make_unique<None>();
}

void ClownScoreBot::OnLog(std::string message) {
    Println(message.c_str());
}

std::unique_ptr<UserState> None::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    if (StartsWith(message->text, "/assign")) {
        auto users = bot.JoinedUsers(message->chat->id);

        bot.Join(message->chat->id, ClownScoreBot::GetUsername(message, query));

        if (!users.size()) {
            bot.SendMessage(message, "There is no users joined in score system, user /join");
            return {};
        }

        auto keyboard = Keyboard::ToKeyboard(users);

        auto result = bot.SendKeyboard(message, "Pick user", keyboard);

        return std::make_unique<WaitForUsername>(result);
    }

    if (StartsWith(message->text, "/join")) {
        bot.Join(message->chat->id, ClownScoreBot::GetUsername(message, query));

        return {};
    }

    if (StartsWith(message->text, "/stats")) {
        int64_t total = bot.Stats(message->chat->id, message->from->username);

        bot.SendMessage(message, Format(u8"% has % 🤡 points", message->from->username, total));

        return {};
    }

    if (StartsWith(message->text, "/rating")) {
        auto rating = bot.Rating(message->chat->id);
        
        std::string result;
    
        for (const auto& [username, total] : rating) {
            result += Format(u8"% has % 🤡 points", username, total) + '\n';
        }

        bot.SendMessage(message, result);

        return {};
    }


    return {};
}

std::unique_ptr<UserState> WaitForUsername::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query) {
    if (!query) {
        bot.SendMessage(message, "Please, pick username first");
        return {};
    }
    bot.AnswerCallbackQuery(query->id);

    bot.DeleteMessage(KeyboardMessage);

    auto result = bot.SendKeyboard(message, "Pick score",
    {
            {
                {"1"},
                {"2"},
                {"3"},
            },
            {
                {"4"},
                {"5"},
                {"6"},
            },
            {
                {"7"},
                {"8"},
                {"9"},
            },
            {
                {"10"},
            }
    });

    return std::make_unique<WaitForScore>(result, query->data);
}

std::unique_ptr<UserState> WaitForScore::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    if (!query) {
        bot.SendMessage(message, "Please, pick score first");
        return {};
    }
    
    bot.DeleteMessage(KeyboardMessage);

    bot.AnswerCallbackQuery(query->id);

    int8_t score = std::atoi(query->data.c_str());

    bot.Assign(message->chat->id, Username, score);

    int64_t total = bot.Stats(message->chat->id, Username);

    bot.SendMessage(message, Format(u8"Added % 🤡 points to %'s score, total % 🤡", (int)score, Username, total));
    
    return std::make_unique<None>();
}
