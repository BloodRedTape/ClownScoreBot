#include "bot.hpp"
#include "format.hpp"
#include "utils.hpp"

int main(int argc, char **argv){

    std::string config_path = "Config.ini";

    if(argc == 2){
        Println("Supplied config path: %", argv[1]);
        config_path = argv[1];
    }

    std::string config_file_content = ReadEntireFile(config_path);

    if (!config_file_content.size()) {
        return Error("Config file is empty or does not exists");
    }

    INIReader config(config_file_content.c_str(), config_file_content.size());

    if(config.ParseError()){
        return Error("Can't parse ini config");
    }


    ClownScoreBot bot(config);

    return bot.Run();
}