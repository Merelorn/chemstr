#include "fg.h"
#include <iostream>
#include <algorithm> // max_element

void FunctionalGroup::orderUp(){
  int current = 0;

  std::vector<int> perm;


  while ( current <= *std::max_element(dist_to_anchor.begin(), dist_to_anchor.end()) ){
    if ( debug ){ std::cerr << "orderUp: " << current << std::endl; }
    for ( int i = 0; i < dist_to_anchor.size(); i++ ){ 
      if ( dist_to_anchor[i] == current ){
    if ( debug ){ std::cerr << "orderUp - dist_to_anchor: " << current << std::endl; }
        if ( debug && dist_to_anchor.size() == 3 ) { std::cerr << "ordering!!! " << dist_to_anchor[i] << " " << i << std::endl;}
        perm.push_back(i);
      }
    }
    current++;
  }
 if ( debug ){ std::cerr << "now reordering" << std::endl; } 
  reorder(indices, perm);
  reorder(ref1, perm);
  reorder(ref2, perm);
  reorder(ref3, perm);
  reorder(distances, perm);
  reorder(angles, perm);
  reorder(dihedrals, perm);
  reorder(correction, perm);
 if ( debug ){ std::cerr << "done reordering" << std::endl; } 
}

FunctionalGroup::FunctionalGroup(const FunctionalGroup & rhs){
  contact = rhs.contact;
//  nucl_id = rhs.nucl_id;
  skeleton_contact = rhs.skeleton_contact;
  way_to_anchor = rhs.way_to_anchor;
  anchor = rhs.anchor;
  indices = rhs.indices; 
  ref1 = rhs.ref1; 
  ref2 = rhs.ref2; 
  ref3 = rhs.ref3; 
  dist_to_anchor = rhs.dist_to_anchor; 
  distances = rhs.distances;
  angles = rhs.angles;
  dihedrals = rhs.dihedrals;
  correction = rhs.correction;

}
