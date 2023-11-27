#include "bot.hpp"
#include "error.hpp"
#include "utils.hpp"
#include <core/os/file.hpp>

#include <vector>
#include "core/string.hpp"

int main(int argc, char **argv){
    String config_path = "Config.ini";

    if(argc == 2){
        Println("Supplied config path: %", argv[1]);
        config_path = String(argv[1]);
    }

    String config_file_content = File::ReadEntire(config_path).Value(String::Empty);

    if (!config_file_content.Size()) {
        return Error("Config file is empty or does not exists");
    }

    Ini config(config_file_content);

    if(config.IsValid()){
        return Errorln("Invalid INI config proveded");
    }

    ClownScoreBot bot(config);

    return bot.Run();
}