#include <algorithm>
#include <props.h>
#include <iostream>
#include "read_in.h"

const float PI_F = 3.14159265;

void Usage(std::string name){
  std::cerr << "Usage: " << name << " xyz/coord-file"
    << std::endl << "\t-n int1 int2 \t\t - to print distance between atom numbers int1 and int2"
    << std::endl << "\t-a int1 int2 int3 \t - to print distance between angle between atom numbers int1-int2-int3"
    << std::endl << "\t-d int1 int2 int3 int4 \t - to print dihedral between atom numbers int1-int2-int3-int4" << std::endl;
}

int main(int argc, char * argv[])
{
  std::string file = "coord";
  bool correct = false;
  int atomnums[4]; 
  
  std::vector<std::string> readArgs;
  std::vector<std::string> allArgs = props::ReadArgs(argc, argv);

  if( props::FindArgs(allArgs, "-f", 1, readArgs) ){ file = readArgs[0]; }
  readArgs.clear();
  Structure str;

  if ( ! props::file_exists(file) || ! str.readSTR(file) ) {
    Usage(argv[0]);
    return 1;
  }

  std::cout.precision(3);
  std::cout.setf(std::ios::fixed,std::ios::floatfield);

  if( props::FindArgs(allArgs, "-n", 2, readArgs) ){
    atomnums[0] = std::stoi(readArgs[0]);
    atomnums[1] = std::stoi(readArgs[1]);
    correct = true;
    std::cout << (str.atoms[atomnums[0] - 1].getXYZ() - str.atoms[atomnums[1] - 1].getXYZ()).norm() << std::endl;
  } else 
  if( props::FindArgs(allArgs, "-a", 3, readArgs) ){
    atomnums[0] = std::stoi(readArgs[0]);
    atomnums[1] = std::stoi(readArgs[1]);
    atomnums[2] = std::stoi(readArgs[2]);
    correct = true;
    Vector3p arm1 = (str.atoms[atomnums[0] - 1].getXYZ() - str.atoms[atomnums[1] - 1].getXYZ());
    Vector3p arm2 = (str.atoms[atomnums[2] - 1].getXYZ() - str.atoms[atomnums[1] - 1].getXYZ());
    std::cout << acos(arm1.dot(arm2)/(arm1.norm() * arm2.norm())) * 360 / ( 2 * PI_F ) << std::endl;
  } else 
  if( props::FindArgs(allArgs, "-d", 4, readArgs) ){
    atomnums[0] = std::stoi(readArgs[0]);
    atomnums[1] = std::stoi(readArgs[1]);
    atomnums[2] = std::stoi(readArgs[2]);
    atomnums[3] = std::stoi(readArgs[3]);
    correct = true;
    Vector3p arm1 = (str.atoms[atomnums[1] - 1].getXYZ() - str.atoms[atomnums[0] - 1].getXYZ());
    Vector3p arm2 = (str.atoms[atomnums[2] - 1].getXYZ() - str.atoms[atomnums[1] - 1].getXYZ());
    Vector3p arm3 = (str.atoms[atomnums[3] - 1].getXYZ() - str.atoms[atomnums[2] - 1].getXYZ());
    Vector3p n1 = (arm1/arm1.norm()).cross((arm2/arm2.norm()));
    Vector3p n2 = (arm2/arm2.norm()).cross((arm3/arm3.norm()));
    std::cout << atan2(n1.cross(arm2/arm2.norm()).dot(n2), n1.dot(n2)) * -1 * 360 / ( 2 * PI_F ) << std::endl;
  } else 
  if ( ! correct ){
    Usage(argv[0]);
    return 1;
  }
   
  return 0;
}
