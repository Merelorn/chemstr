#include "chemstr.h"
#include <props.h>
#include <iostream>

int main(int argc, char* argv[]){

  Structure str;
  if ( argc != 3 || ! props::file_exists(argv[1]) || ! str.readSTR(argv[1]) || props::file_exists(argv[2]) ) {
    std::cerr << "Usage: " << argv[0] << " xyz-source-file xyz-target-file" << std::endl << "  ...to mirrorflip the first structure" << std::endl;
    return 1;
  } else {
    for ( int i = 0; i < str.atoms.size(); i++ ){
      str.atoms[i].setXYZ(-1 * str.atoms[i].getXYZ());
    }
    std::string test = argv[2];
    str.writeXYZ(test);
  }
}

