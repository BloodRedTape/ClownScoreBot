#include "utils.hpp"
#include <fstream>
#include <ctime>
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

std::string ToLowerCase(std::string str){
    return boost::locale::to_lower(str, boost::locale::generator()(""));
}

std::string CurrentDataAndTime(){
    time_t     now = time(nullptr);
    tm  time_struct = *localtime(&now);
    char       buf[80];

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X\0", &time_struct);

    return buf;
}
