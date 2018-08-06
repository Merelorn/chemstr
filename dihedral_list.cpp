#include <algorithm>
#include <props.h>
#include <iostream>
#include "read_in.h"
#include <set>

const float PI_F = 3.14159265;

void Usage(std::string name){
  std::cerr << "Usage: " << name << " coord/XYZ/SDF file" << std::endl;
  std::cerr << "  prints all dihedrals of heavy atoms and their values." << std::endl;
}

int main(int argc, char * argv[])
{
	  std::string file = "coord";

	  bool correct = false;
	  int atomnums[4];
	  Structure str;

	  if ( argc != 2 ){ Usage(argv[0]); }
	  file = argv[1];

	  if ( ! props::file_exists(file) || ! str.readSTR(file) ) {
		Usage(argv[0]);
		return 1;
	  }

	  if ( ! props::ends_with(file, ".sdf") ){
	    str.connectivityBasedOnDistance();
	  }

	  // make all dihedrals
	  std::set<Dihedral> my_dihedrals;
	  Dihedral current;
	  int depth = 0;
	  str.getDihedral(depth, current, my_dihedrals);

	  //list them
	  std::cout.precision(3);
	  std::cout.setf(std::ios::fixed,std::ios::floatfield);

	  Atom * atom_beginning = &(*(str.atoms.begin()));

	  for ( std::set<Dihedral>::iterator it = my_dihedrals.begin(); it != my_dihedrals.end(); it++ ){
		  std::cout << it->atoms[0] - atom_beginning + 1 << " " <<
					it->atoms[1] - atom_beginning + 1 << " " <<
					it->atoms[2] - atom_beginning + 1 << " " <<
					it->atoms[3] - atom_beginning + 1 << " " <<
					str.measure_dihedral(it->atoms[0],it->atoms[1],it->atoms[2],it->atoms[3]) << std::endl;
	  }

	  return 0;
}
