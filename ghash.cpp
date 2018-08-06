#include "read_in.h"
#include <props.h>
#include <iostream>

int main(int argc, char *argv[]){
  Structure str;
  if ( argc != 2 || ! props::file_exists(argv[1]) || ! str.readSTR(argv[1]) ) {
    std::cerr << "Usage: " << argv[0] << " xyz/coord-file" << std::endl << "  ...to print ghash of a structure file" << std::endl;
  } else {
    std::cout.precision(5);
    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    std::cout << 1.889725989 * str.ghash() << std::endl;
    //std::cout << str.ghash() << std::endl;
  }
}
