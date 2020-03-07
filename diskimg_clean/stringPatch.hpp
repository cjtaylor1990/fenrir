//This allows me to convert an integer into a string, overcoming the version problems on galaxy (outdated icc compiler)
#include <string>
#include <sstream>

namespace stringPatch {
    
    template <typename T> std::string toString(const T& input);

}
