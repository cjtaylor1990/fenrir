#include <iostream>
#include <fstream>

//This allows me to convert an integer into a string, overcoming the version problems on galaxy (outdated icc compiler)
#include <string>
#include <sstream>
namespace patch
{
    template < typename T > std::string to_string( const T& nInput )
    {
        std::ostringstream stm ;
        stm << nInput ;
        return stm.str() ;
    }
}

int main(int argc, char* argv[]){
  std::string outFilePrefix = argv[1];
  int caseNumber = atoi(argv[2]);
  int threadCount = atoi(argv[3]);
  std::string fileName;

  std::ofstream outStreams[threadCount];

  int fileIndex = 0;
  while (fileIndex < threadCount){
    fileName = outFilePrefix + "_" + patch::to_string(caseNumber) + "_" + patch::to_string(fileIndex) + ".txt";
    outStreams[fileIndex].open(fileName.c_str());
    fileIndex += 1;
  }

  fileIndex = 0;
  while (fileIndex < threadCount){
    outStreams[fileIndex] << "Hello Thread " << fileIndex << "!\n";
    fileIndex +=1;
  }

  fileIndex = 0;
  while (fileIndex < threadCount){
    outStreams[fileIndex].close();
    fileIndex += 1;
  }

  return 0;
}
