cmake_minimum_required(VERSION 3.10)

project(ClownScoreBot)

add_subdirectory(thirdparty/tgbot-cpp)
add_subdirectory(thirdparty/json)

add_library(inih STATIC
    "./thirdparty/inih/ini.c"
    "./thirdparty/inih/cpp/INIReader.cpp"
)

target_include_directories(inih 
    PRIVATE "./thirdparty/inih"
    PUBLIC "./thirdparty/inih/cpp"
)

add_executable(ClownScoreBot
    "./sources/main.cpp"
    "./sources/bot.cpp"
    "./sources/utils.cpp"
 "sources/score_database.cpp" "sources/simple_bot.cpp")

target_compile_features(ClownScoreBot PUBLIC cxx_std_17)

target_link_libraries(ClownScoreBot
    TgBot
    nlohmann_json
    boost_locale
    inih
)

target_include_directories(ClownScoreBot PRIVATE "./sources")