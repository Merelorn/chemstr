#include "chemstr.h"
#include <algorithm>	// std::equal
#include <iomanip>	// ios::setprecision
#include <stdlib.h>	// atoi
#include <props.h>	// ReadColumn, ends_with, file_exists
#include <iostream>
#include <iterator>	// std::next

const float A2AU = 1.889725989; // Angstrom to atomic units
bool debug = false;

// Atom - START
Atom::Atom(){
  set = false;
  type = "";
  xyz = Vector3p(0.0, 0.0, 0.0);
}

Atom::Atom(Vector3p xyz, std::string type) : xyz(xyz), type(type), set(true){}
 
Vector3p Atom::getXYZ() const { return xyz; }
std::string Atom::getTYPE(){ return type; }

void Atom::setXYZ(precision x, precision y, precision z){
  xyz = Vector3p(x,y,z);
}

void Atom::setXYZ(Vector3p ref){ xyz = ref; }
// Atom - END

// Structure - START
Structure::Structure(){connected = false;};

Structure::Structure(const Structure & rhs){
  // we need to redirect pointers to our new structure
  atoms = rhs.atoms;
  connected = true;
  for ( int i = 0; i < rhs.atoms.size(); i++){
    atoms[i].partners.clear();
    for ( int j = 0; j < rhs.atoms[i].partners.size(); j++){
      atoms[i].partners.insert(&atoms[*std::next(rhs.atoms[i].partners.begin(),j) - &rhs.atoms[0]]);
      // we have the pointers that point to rhs.atoms. we assign them here to point to same places relatively as in rhs
    }
  }
}

Structure::Structure(std::string file){
  readSTR(file);
}

bool Structure::readSTR(std::string file){
  atoms.clear();
  connected = false;
  if ( props::ends_with(file, ".xyz") ){ return(readXYZ(file)); }
  if ( props::ends_with(file, ".cosmo") ){ return(readCOSMO(file)); }
  if ( props::ends_with(file, ".sdf") ){ return(readSDF(file)); }
//  if ( props::ends_with(file, ".tmol") || props::ends_with(file, "coord") ){ return(readTMOL(file)); }
  return (readTMOL(file));
//  return false;
}

bool Structure::readXYZ(std::string file){
  if ( ! props::file_exists(file) ){ return false; }
  if ( ! props::ends_with(file, ".xyz") ){ return false; }

  std::ifstream myofile(file);
  std::string line;
 
  std::getline(myofile,line); // Line 1 - number of atoms
  int expected_no_atoms = atoi(line.c_str());
  
  std::getline(myofile,line); // Line 2 - title line (ignored)

  while ( myofile.good() ){
    std::getline(myofile,line); // Line 2 - title line (ignored)
    if ( line != "" ){
      atoms.push_back(Atom(Vector3p(props::ReadColumnP(line,2), props::ReadColumnP(line, 3), props::ReadColumnP(line, 4)), props::ReadColumn(line, 1)));
    }
  }

  myofile.close();
  return true;
}

bool Structure::readSDF(std::string file){
  if ( ! props::file_exists(file) ){ return false; }
  if ( ! props::ends_with(file, ".sdf") ){ return false; }

  std::ifstream myofile(file);
  std::string line;

  std::getline(myofile,line); // Line 1 - title
  std::getline(myofile,line); // Line 2 - comment line
  std::getline(myofile,line); // Line 3 - empty line

  std::getline(myofile,line); // Line 4 - number of atoms and bonds
  int expected_no_atoms = atoi(props::ReadColumn(line,1).c_str());
  int expected_no_bonds = atoi(props::ReadColumn(line,2).c_str());

  for ( int i = 0; i < expected_no_atoms; i++){
	    std::getline(myofile,line);
	    atoms.push_back(Atom(Vector3p(props::ReadColumnP(line,1), props::ReadColumnP(line, 2), props::ReadColumnP(line, 3)), props::ReadColumn(line, 4)));
  }

  int first, second; // atomids of a bond
  for ( int i = 0; i < expected_no_bonds; i++){
	    std::getline(myofile,line); // Line 2 - title line (ignored)
	    first = atoi(props::ReadColumn(line,1).c_str());
	    second = atoi(props::ReadColumn(line,2).c_str());
	    addPartner(first, second);
  }

  myofile.close();
  connected = true;
  return true;
}

bool Structure::readTMOL(std::string file){
  if ( ! props::file_exists(file) ){ return false; }
//  if ( ! props::ends_with(file, ".tmol") && ! props::ends_with(file, "coord") ){ return false; }

  std::ifstream myofile(file);
  std::string line;
 
  std::getline(myofile,line); // Line 1 - '$coord' - ignored

  while ( myofile.good() ){
    std::getline(myofile,line); // Line 2 - title line (ignored)
    if ( line == "" || props::begins_with(line,"$") ){
      break;
    } else {
      atoms.push_back(Atom(Vector3p(props::ReadColumnP(line,1), props::ReadColumnP(line, 2), props::ReadColumnP(line, 3))/A2AU, props::ReadColumn(line, 4)));
    }
  }

  myofile.close();
  return true;
}

bool Structure::readCOSMO(std::string file){
  if ( ! props::file_exists(file) ){ return false; }
//  if ( ! props::ends_with(file, ".tmol") && ! props::ends_with(file, "coord") ){ return false; }

  std::ifstream myofile(file);
  std::string line;
 
  bool coord_start = false;

  while ( myofile.good() ){
    std::getline(myofile,line); // read until beginning of coords
    if ( line == "$coord_rad" ){
      std::getline(myofile,line); // read #atom x y z element radius [A] line...
      coord_start = true;
      break;
    } 
  }

  if ( ! coord_start ){ return false; }
  coord_start = false;

  while ( myofile.good() ){
    std::getline(myofile,line); 
    if ( line == "$coord_car" ){
      coord_start = true;
      break;
    } else {
      atoms.push_back(Atom(Vector3p(props::ReadColumnP(line,2), props::ReadColumnP(line, 3), props::ReadColumnP(line, 4))/A2AU, props::ReadColumn(line, 5)));
    }
  }

  myofile.close();
  if ( ! coord_start ){ return false; }
  return true;
}
  
int Atom::period() const{
  std::string newtype = type;
  std::transform(newtype.begin(), newtype.end(), newtype.begin(), ::tolower);
  
  if ( newtype == "" ){ return 0; }

  if ( newtype == "h" ){ return 1; }

  if ( newtype == "li" ){ return 2; }
  if ( newtype == "be" ){ return 2; }
  if ( newtype == "b" ){ return 2; }
  if ( newtype == "c" ){ return 2; }
  if ( newtype == "n" ){ return 2; }
  if ( newtype == "o" ){ return 2; }
  if ( newtype == "f" ){ return 2; }
  if ( newtype == "h" ){ return 2; }
  if ( newtype == "ne" ){ return 2; }

  if ( newtype == "na" ){ return 3; }
  if ( newtype == "mg" ){ return 3; }
  if ( newtype == "al" ){ return 3; }
  if ( newtype == "si" ){ return 3; }
  if ( newtype == "p" ){ return 3; }
  if ( newtype == "s" ){ return 3; }
  if ( newtype == "cl" ){ return 3; }
  if ( newtype == "ar" ){ return 3; }

  return 4;

}

int Atom::valence() const {
  std::string newtype = type;
  std::transform(newtype.begin(), newtype.end(), newtype.begin(), ::tolower);
  
  if ( newtype == "h" ){ return 1; }
  if ( newtype == "c" ){ return 4; }
  if ( newtype == "n" ){ return 3; }
  if ( newtype == "o" ){ return 2; }
  if ( newtype == "f" ){ return 1; }
  if ( newtype == "cl" ){ return 1; }
 
  return 10;
}

precision Structure::distThreshold(int i, int j){
  switch(std::min(atoms[i].period(), atoms[j].period())){
    case 1:
      return 1.2;
    case 2:
      return 1.98;
    case 3:
      return 2.3;
    
  }
}

void Structure::connectivityBasedOnDistance(){
  precision dist_thr;

  for ( int i = 0; i < atoms.size() - 1; i++ ){
    for ( int j = 0; j < atoms.size(); j++){
      if ( i == j ){ continue; }
      dist_thr = distThreshold(i,j);
      if ( abs(std::min(atoms[i].period(), atoms[j].period()) - std::max(atoms[i].period(), atoms[j].period()) ) > 1 ){ dist_thr += 0.3; }
      if ( (atoms[i].xyz - atoms[j].xyz).norm() < dist_thr ) { 
	addPartner(i+1,j+1); //addPartner decrements by 1
      }
    }
  }
  dropConflictsBasedOnDistance();
  connected = true;
}


void Structure::dropConflictsBasedOnDistance(){
  int i = 0;
  
  do{
    if ( atoms[i].partners.size() > atoms[i].valence() ){
      precision max_dist = 0;
      precision current_dist = 0;
      int max_index;
      for ( auto it : atoms[i].partners ){
        current_dist = ( atoms[i].xyz - it->xyz ).norm();
        if ( current_dist > max_dist ){ max_dist = current_dist; max_index = it - &atoms[0]; }
      }
      remPartner(i + 1, max_index + 1); // remPartner decrements by 1
      i = -1;
    }
  i++;
  } while ( i < atoms.size() );

}


int Structure::distanceOverBonds(int first, int second, int max_depth){ // BFS Breadth-first search, requires connectivity;
  if ( !connected ){ return -1; }
  if ( first > atoms.size() || first < 1 || second > atoms.size() || second < 1 ){ return -1; } // out of bounds?
  int distance = 0;

  Atom * target = &atoms[second - 1];  // define target - atom "second"
  std::set<Atom*> current_layer;
  current_layer.insert(&atoms[first - 1]); // initialize with atom "first"
  std::set<Atom*> next_layer; // here we store new layers - neighbours of atoms in the current layer
  std::set<Atom*> visited; // here we store visited nodes - we need to avoid visiting them again

  while ( distance <= max_depth ){ // in case of disconnected molecule we will stop after atoms.size() steps
    for ( std::set<Atom*>::iterator it = current_layer.begin(); it != current_layer.end(); it++ ){
      if ( *it == target ){ return distance; }
      visited.insert(*it); 
      
      for ( std::set<Atom*>::iterator it2 = (*it)->partners.begin(); it2 != (*it)->partners.end(); it2++ ){
        if ( std::find(visited.begin(), visited.end(), *it2) == visited.end() ) { next_layer.insert(*it2); }
      }    
    }
    current_layer = next_layer;
    next_layer.clear();
    distance++;
  }
  return -1;
}

// Make sure this is the shortest way
int Structure::pathFinder(int first, int second, std::vector<int> & path, int max_depth, bool cyclic, Atom * parent, int distance){
  if ( !connected ){ return -1; }
  if ( first > atoms.size() || first < 1 || second > atoms.size() || second < 1 ){ return -1; } // out of bounds?

  if ( max_depth == -1 ){ max_depth = distanceOverBonds(first, second, 999);} 
  if ( distance != 0 ) {path.push_back(first);}
  if ( first == second ){ return distance; }
  if ( distance == max_depth ) { return -1; }

  int found; 

  for ( std::set<Atom*>::iterator it = atoms[first - 1].partners.begin(); it != atoms[first - 1].partners.end(); it++ ){
    if ( *it == parent ){ continue; } // Do not go back to parent
    found = pathFinder(*it - &atoms[0] + 1, second, path, max_depth, false, &atoms[first - 1], distance + 1);
    if ( found != -1 ){ return found; } else { path.pop_back(); }
  }
  return -1;
}

precision Structure::ghash(){
  precision sum = 0;
  
  for ( auto it : atoms ){
    sum += (atoms.begin()->getXYZ() - it.getXYZ()).norm();
  } 
  return sum;

}

bool Structure::addPartner(int first, int second){
	if ( first > atoms.size() ){ return false; }
	if ( second > atoms.size() ){ return false; }

	atoms[first-1].partners.insert(&atoms[second-1]);
	atoms[second-1].partners.insert(&atoms[first-1]);
        return true;

}

bool Structure::remPartner(int first, int second){
  if ( first > atoms.size() ){ return false; }
  if ( second > atoms.size() ){ return false; }

  Atom * first_p = &atoms[first - 1];
  Atom * second_p = &atoms[second - 1];

  if ( atoms[first - 1].partners.find(second_p) == atoms[first - 1].partners.end() ){ return false; }
  if ( atoms[second - 1].partners.find(first_p) == atoms[second - 1].partners.end() ){ return false; }

  std::set<Atom*>::iterator del_it;
  del_it = atoms[first - 1].partners.find(second_p); atoms[first - 1].partners.erase(del_it);
  del_it = atoms[second - 1].partners.find(first_p); atoms[second - 1].partners.erase(first_p);

  return true;
}

bool Structure::identicalConnectivity(const Structure & rhs){
  if ( ! connected || ! rhs.connected ){ return false; }
  if ( atoms.size() != rhs.atoms.size() ){ return false; }

  std::set<int> partners, rhs_partners;
  
  for ( int i = 0; i < atoms.size(); i++ ){
    partners.clear(); rhs_partners.clear();
    for ( std::set<Atom*>::iterator it = atoms[i].partners.begin(); it != atoms[i].partners.end(); it++) {
      partners.insert(*it - &atoms[0] + 1);
    }
    for ( std::set<Atom*>::iterator it = rhs.atoms[i].partners.begin(); it != rhs.atoms[i].partners.end(); it++) {
      rhs_partners.insert(*it - &rhs.atoms[0] + 1);
    }

    if ( partners != rhs_partners ){
      if ( debug ){
        std::cerr << i + 1 << std::endl;
        for ( auto it : partners ){ std::cerr << it << " "; }; std::cerr << std::endl; 
        for ( auto it : rhs_partners ){ std::cerr << it << " "; }; std::cerr << std::endl;
      }
      return false;
    }
  }

  return true;
}

precision Structure::measure_dihedral(int a1, int a2, int a3, int a4) const {
  return measure_dihedral(&atoms[a1], &atoms[a2], &atoms[a3], &atoms[a4]);
}

precision Structure::measure_angle(int a1, int a2, int a3) const{
  return measure_angle(&atoms[a1], &atoms[a2], &atoms[a3]);
}

precision Structure::measure_distance(int a1, int a2) const{
  return measure_distance(&atoms[a1], &atoms[a2]);
}

precision Structure::measure_dihedral(const Atom* a1, const Atom* a2, const Atom* a3, const Atom* a4) const{
    Vector3p arm1 = (a2->getXYZ() - a1->getXYZ());
    Vector3p arm2 = (a3->getXYZ() - a2->getXYZ());
    Vector3p arm3 = (a4->getXYZ() - a3->getXYZ());
    Vector3p n1 = (arm1/arm1.norm()).cross((arm2/arm2.norm()));
    Vector3p n2 = (arm2/arm2.norm()).cross((arm3/arm3.norm()));
    return atan2(n1.cross(arm2/arm2.norm()).dot(n2), n1.dot(n2)) * -1 * 360 / ( 2 * M_PI );
}

precision Structure::measure_angle(const Atom* a1, const Atom* a2, const Atom* a3) const{
    Vector3p arm1 = (a1->getXYZ() - a2->getXYZ());
    Vector3p arm2 = (a3->getXYZ() - a2->getXYZ());
    return acos(arm1.dot(arm2)/(arm1.norm()*arm2.norm())) * 180 / M_PI;
}

precision Structure::measure_distance(const Atom* a1, const Atom* a2) const{
    return (a1->getXYZ() - a2->getXYZ()).norm();
}

void Structure::getDihedral(int depth, Dihedral & current, std::set<Dihedral> & my_dihedrals, bool unique){
  bool redundant = false;
  if ( depth == 0 ){
    for ( std::vector<Atom>::iterator it = atoms.begin(); it != atoms.end(); it++ ){
      if ( it->type == "h" || it->type == "H" ){continue;} // we don't care about dihedrals involving hydrogens
      current.atoms[depth] = &(*it);
      getDihedral(depth+1, current, my_dihedrals, unique);
    }
  } else  if ( depth <= 3 ){
    for ( std::set<Atom*>::iterator it = current.atoms[depth-1]->partners.begin(); it != current.atoms[depth-1]->partners.end(); it++){
        if ( (*it)->type == "h" || (*it)->type == "H" ){continue;} // we don't care about dihedrals involving hydrogens
        if ( depth == 2 && (*it) == current.atoms[0] ){ continue;} // if we get into a cycle, skip this
        if ( depth == 3 && (*it) == current.atoms[0] ){ continue;} // if we get into a cycle, skip this
        if ( depth == 3 && (*it) == current.atoms[1] ){ continue;} // if we get into a cycle, skip this

        current.atoms[depth] = *it;
        if ( depth != 3){
        	getDihedral(depth+1, current, my_dihedrals, unique);
        } else {
        	redundant = false;
        	for ( std::set<Dihedral>::iterator it = my_dihedrals.begin(); it != my_dihedrals.end(); it++ ){
        		if ( (*it) == current ) { redundant = true; break; }
			if ( unique ){
				if ( ( current.atoms[1] == it->atoms[1] && current.atoms[2] == it->atoms[2] ) || ( current.atoms[1] == it->atoms[2] && current.atoms[2] == it->atoms[1] ) ){
					redundant = true; break;
				} 
			}
        	}
        	if ( !redundant ){my_dihedrals.insert(current);}
        }
    }
  }
}

void Structure::getAngle(int depth, Angle & current, std::set<Angle> & my_angles){
  bool redundant = false;
  if ( depth == 0 ){
    for ( std::vector<Atom>::iterator it = atoms.begin(); it != atoms.end(); it++ ){
      if ( it->type == "h" || it->type == "H" ){continue;} // we don't care about angles involving hydrogens
      current.atoms[depth] = &(*it);
      getAngle(depth+1, current, my_angles);
    }
  } else  if ( depth <= 3 ){
    for ( std::set<Atom*>::iterator it = current.atoms[depth-1]->partners.begin(); it != current.atoms[depth-1]->partners.end(); it++){
        if ( (*it)->type == "h" || (*it)->type == "H" ){continue;} // we don't care about dihedrals involving hydrogens
        if ( depth == 2 && (*it) == current.atoms[0] ){ continue;} // if we get into a cycle, skip this

        current.atoms[depth] = *it;
        if ( depth != 3){
        	getAngle(depth+1, current, my_angles);
        } else {
        	redundant = false;
        	for ( std::set<Angle>::iterator it = my_angles.begin(); it != my_angles.end(); it++ ){
        		if ( (*it) == current ) { redundant = true; }
        	}
        	if ( !redundant ){my_angles.insert(current);}
        }
    }
  }
}


bool Structure::writeXYZ(std::string file, bool append){
        std::string line;
        std::ofstream myofile;
        myofile.open(file);

        if ( ! atoms.size() != 0 ){ return false;}

        myofile << props::tostring(int(atoms.size())) << std::endl;
        myofile << std::endl;
        myofile << std::setprecision(5) << std::fixed;

        for ( int i = 0; i < atoms.size(); i++){
                myofile.width(3); myofile << std::left << atoms[i].getTYPE();
                myofile.width(15); myofile << std::right << atoms[i].getXYZ().x();
                myofile.width(15); myofile << std::right << atoms[i].getXYZ().y();
                myofile.width(15); myofile << std::right << atoms[i].getXYZ().z();
                myofile << std::endl;
        }
        myofile.close();
        return true;
}

bool Structure::InsideCycle(int atom_id, int cycle_size, std::set<int> & indices){
  indices.clear();
  int index;
  std::vector<int> v_indices;

  for ( std::set<Atom*>::iterator it = atoms[atom_id - 1].partners.begin(); it != atoms[atom_id - 1].partners.end(); it++){
    v_indices.clear();
    index = *it - &atoms[0] + 1;
    Structure alt(*this);
    alt.remPartner(atom_id, index);

    if ( alt.pathFinder(index, atom_id, v_indices, cycle_size - 1) == cycle_size - 1 ){
      for ( auto it : v_indices ){ indices.insert(it); }
      indices.insert(index);
      return true;
    }
  }
  return false;
}



bool Dihedral::operator<(const Dihedral & rhs) const {
	for (int i = 0; i < 4; i++){
		if ( atoms[i] > rhs.atoms[i] ){ return false; }
		if ( atoms[i] < rhs.atoms[i] ){ return true; }
	}
	return false;
}

bool Dihedral::operator>(const Dihedral & rhs) const {
	return (rhs < *this);
}

bool Dihedral::operator==(const Dihedral & rhs) const {

	bool forward = true;
	bool backward = true;

	for (int i = 0; i < 4; i++){
		if ( this->atoms[i] != rhs.atoms[i] ){ forward = false; }
	}

	for (int i = 0; i < 4; i++){
		if ( this->atoms[i] != rhs.atoms[3-i] ){ backward = false; }
	}

	return ( forward || backward );
}

bool Angle::operator<(const Angle & rhs) const {
	for (int i = 0; i < 3; i++){
		if ( atoms[i] > rhs.atoms[i] ){ return false; }
		if ( atoms[i] < rhs.atoms[i] ){ return true; }
	}
	return false;
}

bool Angle::operator>(const Angle & rhs) const {
	return (rhs < *this);
}

bool Angle::operator==(const Angle & rhs) const {

	bool forward = true;
	bool backward = true;

	for (int i = 0; i < 3; i++){
		if ( this->atoms[i] != rhs.atoms[i] ){ forward = false; }
	}

	for (int i = 0; i < 3; i++){
		if ( this->atoms[i] != rhs.atoms[2-i] ){ backward = false; }
	}

	return ( forward || backward );
}



//bool Structure::writeTMOL(std::string file, append = false);

// Structure - END

