#pragma once
#include "chemstr.h"
#include <vector>

class FunctionalGroup{
public:

//  FunctionalGroup(int contact, int skeleton_contact, int nucl_id) : contact(contact), skeleton_contact(skeleton_contact), nucl_id(nucl_id){};
  FunctionalGroup(int contact, int skeleton_contact) : contact(contact), skeleton_contact(skeleton_contact){};
  FunctionalGroup(const FunctionalGroup & rhs);

  std::vector<int> indices; // indices of atoms in isolated groups
  std::vector<int> ref1; // indices of atoms in isolated groups
  std::vector<int> ref2; // indices of atoms in isolated groups
  std::vector<int> ref3; // indices of atoms in isolated groups

  std::vector<int> dist_to_anchor; // distances to anchor - used to rearrange all vectors by increasing dist_to_anchor
  void orderUp();  

template< class T > inline void reorder(std::vector<T> & v, std::vector<int> const & perm )  {   
  std::vector<T> answer;
//  std::cerr << v.size() << " " << perm.size() << std::endl;
  for( int j = 0; j < perm.size(); j++){
    answer.push_back(v[perm[j]]);
  } 
  v = answer;
}

  int skeleton_contact; // distance 1 to contact (on a skeleton)
  int way_to_anchor; // distance 2 to contact (on a skeleton) - important for correction
  int anchor; // distance 3 to contact (on a skeleton)
  int contact; // isolated atom directly bonded to skeleton
//  int nucl_id; // belong to nucleotide with ID (1 or 2)

  // positions of atoms (indices) is defined in z-matrix in relation to the 3 skeleton_reference atoms
  std::vector<precision> distances; 
  std::vector<precision> angles;
  std::vector<precision> dihedrals;
  std::vector<precision> correction; // value of dihedral correction value - non-zero for contact only; 
};

