#include <algorithm>
#include <props.h>
#include <iostream>
#include "read_in.h"
#include "recognize.h"
#include <set>
#include <stdlib.h>

const float PI_F = 3.14159265;
extern bool debug;

void Usage(std::string name){
  std::cerr << "Usage: " << name << "file1.xyz" << std::endl;
  std::cerr << "  Try reading-in a CDN molecule and print its universal and skeleton maps." << std::endl;
}

int main(int argc, char * argv[])
{

std::vector<std::string> args(argv, argv + argc);
if ( debug ){ std::cerr << "starting " << std::endl; }

  if ( argc != 3 && argc != 4 ){ Usage(argv[0]); return 1; }

  if ( argc > 3 ){
    for ( std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++){
      if ( *it == "-d" ){
        debug = true;
      }
    }
  }

  std::string file = args[1];
  std::string file2 = args[2];

  Structure str, str2;

  if ( ! props::file_exists(file) || !str.readSTR(file) || !str2.readSTR(file2) ) {
	Usage(argv[0]);
	return 1;
  }

  str.connectivityBasedOnDistance();
  str2.connectivityBasedOnDistance();

  if ( str.identicalConnectivity(str2) ){ return 0; } else { return 1; }

}

