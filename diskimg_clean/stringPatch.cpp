//This allows me to convert an integer into a string, overcoming the version problems on galaxy (outdated icc compiler)
#include <string>
#include <sstream>
#include "stringPatch.hpp"

using namespace stringPatch;

template <typename T> std::string stringPatch::toString (const T& input){
    std::ostringstream steam;
    stream << input;
    return stream.str();
}
