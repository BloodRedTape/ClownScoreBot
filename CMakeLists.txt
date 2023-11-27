cmake_minimum_required(VERSION 3.10)

project(ClownScoreBot)

add_subdirectory(thirdparty/straitx-json)
add_subdirectory(thirdparty/straitx-core)
add_subdirectory(thirdparty/straitx-tgbot)
add_subdirectory(thirdparty/straitx-ini)

add_executable(ClownScoreBot
    "./sources/main.cpp"
    "./sources/bot.cpp"
    "./sources/utils.cpp"
    "./sources/score_database.cpp" 
)

target_compile_features(ClownScoreBot PUBLIC cxx_std_17)

target_link_libraries(ClownScoreBot
    boost_locale
    StraitXCore
    StraitXTgBot
    StraitXJson
    StraitXIni
)

target_include_directories(ClownScoreBot PRIVATE "./sources")