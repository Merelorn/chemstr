#include <algorithm>
#include <props.h>
#include <iostream>
#include "chemstr.h"
#include <set>
#include <stdlib.h>

const float PI_F = 3.14159265;

precision cyclic_distance(precision val1, precision val2){ // return distance in degrees of two dihedrals (-180 = 180)
	if ( ( val1 < 0 && val2 < 0 ) || ( val1 > 0 && val2 > 0 ) ){ return abs(val1-val2); }
	precision new1 = val1;
	precision new2 = val2;
	if ( val1 < 0 ){ new1 = val1 + 360; }
	if ( val2 < 0 ){ new2 = val2 + 360; }
	return std::min(abs(val1-val2), abs(new1-new2));
}

void Usage(std::string name){
  std::cerr << "Usage: " << name << "-f file1 file2 -t threshold_in_degrees" << std::endl;
  std::cerr << "  print number of dihedrals differing by more than $threshold" << std::endl;
}

int main(int argc, char * argv[])
{
	 std::string file1, file2;
	 Structure str1, str2;
	 bool correct = false;
	 int atomnums[4];
	 precision threshold = 30;

	 std::vector<std::string> readArgs;
	 std::vector<std::string> allArgs = props::ReadArgs(argc, argv);

	 if( props::FindArgs(allArgs, "-f", 2, readArgs) ){
		 file1 = readArgs[0];
		 file2 = readArgs[1];
	 }
	 readArgs.clear();

	 bool report_sum = false;
	 if( props::FindArgs(allArgs, "-sum", 0, readArgs) ){
		 report_sum = true;
	 }
	 readArgs.clear();


	  if ( ! props::file_exists(file1) || ! str1.readSTR(file1) ) {
		Usage(argv[0]);
		return 1;
	  }
	  if ( ! props::ends_with(file1, ".sdf") ){ str1.connectivityBasedOnDistance(); } 

	  if ( ! props::file_exists(file2) || ! str2.readSTR(file2) ) {
		Usage(argv[0]);
		return 2;
	  }
	  if ( ! props::ends_with(file2, ".sdf") ){ str2.connectivityBasedOnDistance(); } 

	 if( props::FindArgs(allArgs, "-t", 1, readArgs) ){ threshold = std::stof(readArgs[0].c_str()); }

  std::vector<std::pair<int,int> > ignore_pairs;
  if( props::FindArgs(allArgs, "-i", -1, readArgs) ){
    if ( readArgs.size() % 2 == 1 ){ Usage(argv[0]); return 3; }
    for ( std::vector<std::string>::iterator it = readArgs.begin(); it != readArgs.end(); std::advance(it,2) ){
       ignore_pairs.push_back( std::pair<int,int> (atoi(it->c_str()), atoi(std::next(it)->c_str())) );
    }
  }

  bool verbose = false;
  if( props::FindArgs(allArgs, "-v", 0, readArgs) ){ verbose = true; }


  // make all dihedrals
  std::set<Dihedral> my_dihedrals1, my_dihedrals2;
  Dihedral current;
  int depth = 0;
  str1.getDihedral(depth, current, my_dihedrals1);
  str2.getDihedral(depth, current, my_dihedrals2);

  if ( my_dihedrals1.size() != my_dihedrals2.size() ){ std::cerr << "Different number of dihedrals!" << std::endl; std::cout << "NA" << std::endl; }

  int count = 0;
  precision dihed1, dihed2, errorsum = 0;

  std::set<Dihedral>::iterator it2 = my_dihedrals2.begin();


  for (std::set<Dihedral>::iterator it = my_dihedrals1.begin(); it != my_dihedrals1.end(); it++,it2++){
	  int mid1 = it->atoms[1] - &str1.atoms[0] + 1;
	  int mid2 = it->atoms[2] - &str1.atoms[0] + 1;
	  if ( std::find(ignore_pairs.begin(), ignore_pairs.end(), std::pair<int,int>(mid1,mid2)) != ignore_pairs.end() || std::find(ignore_pairs.begin(), ignore_pairs.end(), std::pair<int,int>(mid2,mid1)) != ignore_pairs.end() ){ continue; } // if current dihedral is one of the ignore pairs ( given by "-i" option ) - ignore it
	  dihed1 = str1.measure_dihedral(it->atoms[0],it->atoms[1],it->atoms[2],it->atoms[3]);
	  dihed2 = str2.measure_dihedral(it2->atoms[0],it2->atoms[1],it2->atoms[2],it2->atoms[3]);
	  errorsum += cyclic_distance(dihed1, dihed2);
	  if ( cyclic_distance(dihed1, dihed2) > threshold ){
	    count++;
	    if ( verbose ){
              std::cout << it->atoms[0] - &str1.atoms[0] + 1 << " ";
              std::cout << it->atoms[1] - &str1.atoms[0] + 1 << " ";
              std::cout << it->atoms[2] - &str1.atoms[0] + 1 << " ";
              std::cout << it->atoms[3] - &str1.atoms[0] + 1 << " ";
	      std::cout << std::endl;
	    }	
	  }
  }
  if ( report_sum ){
	  std::cout << errorsum << " " << std::endl;
  } else {
  if ( ! verbose ){ std::cout << count << " " << std::endl; }
  }
  //std::cout << my_dihedrals1.size() << " " << count << " " << errorsum << std::endl;
  return 0;
}
