#include "bot.hpp"
#include <exception>
#include <core/unicode.hpp>
#include "utils.hpp"
#include "error.hpp"

const char *CancelButton = SX_UTF8("[Cancel]");

ClownScoreBot::ClownScoreBot(const Ini &config):
    SimpleBot(
        config.Get(SectionName, "Token")
    ),
    JsonDatabase(
        config
    )
{
    BindOnUnknownCommand({this, &ClownScoreBot::OnUnknownCommand});
    BindOnCallbackQuery({this, &ClownScoreBot::OnCallbackQuery});
    BindOnLog({this, &ClownScoreBot::OnLog});
}

void ClownScoreBot::UpdateUserState(TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    String username = GetUsername(message, query);

    EnsureUserState(username);

    auto &state = m_Users[username];

    auto new_state = state->Update(*this, message, query);

    if (new_state){
        state = Move(new_state);
    }
}

void ClownScoreBot::EnsureUserState(const String& username) {
    if(!m_Users.Has(username))
        m_Users[username] = MakeUnique<None>();
}

void ClownScoreBot::OnLog(String message) {
    Println("%", message);
}

UniquePtr<UserState> None::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    if (StartsWith(message->text, "/assign")) {
        auto users = bot.JoinedUsers(message->chat->id);

        bot.Join(message->chat->id, ClownScoreBot::GetUsername(message, query));

        if (!users.Size()) {
            bot.SendMessage(message, "There is no users in score system, use /join");
            return {};
        }

        auto keyboard = Keyboard::ToKeyboard(users);

        keyboard.Add({ {CancelButton} });

        auto result = bot.SendKeyboard(message, "Pick user", keyboard);

        return MakeUnique<WaitForUsername>(result, Move(message->from->username));
    }

    if (StartsWith(message->text, "/join")) {
        bot.Join(message->chat->id, ClownScoreBot::GetUsername(message, query));

        return {};
    }

    if (StartsWith(message->text, "/stats")) {
        int64_t total = bot.Stats(message->chat->id, String::FromStdString(message->from->username));

        bot.SendMessage(message, Format(SX_UTF8("% has % 🤡 points"), message->from->username, total));

        return {};
    }

    if (StartsWith(message->text, "/rating")) {
        auto rating = bot.Rating(message->chat->id).ToList();

        std::sort(rating.begin(), rating.end(), [](auto& left, auto &right) {
            return left.Second > right.Second; 
        });
        
        String result;
    
        for (const auto& [username, total] : rating) {
            result += Format(SX_UTF8("% has % 🤡 points"), username, total) + '\n';
        }

        bot.SendMessage(message, result);

        return {};
    }


    return {};
}

UniquePtr<UserState> WaitForUsername::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query) {
    if (!query) {
        bot.SendMessage(message, "Please, pick username first");
        return {};
    }

    bot.AnswerCallbackQuery(query->id);

    if (query->data == CancelButton) {
        bot.DeleteMessage(KeyboardMessage);
        return MakeUnique<None>();
    }

    bot.EditMessage(KeyboardMessage, "Pick score",
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
            },
            {
                {CancelButton}
            }
    });

    return MakeUnique<WaitForScore>(KeyboardMessage, Move(InitiatorUsername), Move(query->data));
}

UniquePtr<UserState> WaitForScore::Update(ClownScoreBot& bot, TgBot::Message::Ptr message, TgBot::CallbackQuery::Ptr query){
    if (!query) {
        bot.SendMessage(message, "Please, pick score first");
        return {};
    }

    bot.AnswerCallbackQuery(query->id);
    
    bot.DeleteMessage(KeyboardMessage);

    if (query->data == CancelButton) {
        return MakeUnique<None>();
    }

    int8_t score = std::atoi(query->data.c_str());

    bot.Assign(message->chat->id, TargetUsername, score);

    int64_t total = bot.Stats(message->chat->id, TargetUsername);
    
    bool self_added = InitiatorUsername == TargetUsername;

    String reply = self_added 
        ? Format(SX_UTF8("Added % 🤡 points to %'s score, total % 🤡"), (int)score, TargetUsername, total)
        : Format(SX_UTF8("% added % 🤡 points to %'s score, total % 🤡"), InitiatorUsername, (int)score, '@' + TargetUsername, total);

    bot.SendMessage(message, reply);
    
    return MakeUnique<None>();
}
