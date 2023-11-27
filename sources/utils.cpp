#include "utils.hpp"
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

std::string ToLowerCase(std::string str){
    return boost::locale::to_lower(str, boost::locale::generator()(""));
}
