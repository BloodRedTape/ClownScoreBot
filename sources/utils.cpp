#include "utils.hpp"
#include <fstream>
#include <ctime>
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

std::string ToLowerCase(std::string str){
    return boost::locale::to_lower(str, boost::locale::generator()(""));
}

int CountChars(char ch, const char *string){
    int count = 0;
    while(*string){
        char e = *string++;
        count += e == ch;
    }
    return count;
}

std::string ReadEntireFile(const std::string &filepath){
    std::ifstream t(filepath);
    if(!t.is_open()){
        printf("Can't read '%s'\n", filepath.c_str());
        return {};
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size); 
    return buffer;
}

void WriteEntireFile(const std::string &filepath, const std::string &content){
    std::ofstream stream(filepath);
    if(!stream.is_open())
        return (void)printf("Can't write '%s'\n", filepath.c_str());
    stream.write(content.data(), content.size());
}


std::string CurrentDataAndTime(){
    time_t     now = time(nullptr);
    tm  time_struct = *localtime(&now);
    char       buf[80];

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X\0", &time_struct);

    return buf;
}
