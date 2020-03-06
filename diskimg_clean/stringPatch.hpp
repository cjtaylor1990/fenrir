//This allows me to convert an integer into a string, overcoming the version problems on galaxy (outdated icc compiler)
#include <string>
#include <sstream>
namespace string_patch
{
    template < typename T > std::string to_string( const T& nInput )
    {
        std::ostringstream stm ;
        stm << nInput ;
        return stm.str() ;
    }
}
