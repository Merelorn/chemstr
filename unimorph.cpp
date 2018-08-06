#include <algorithm>
#include <iostream>
#include <iomanip>
#include "read_in.h"
#include "hung.h"
#include <vector>
#include <props.h>
#include "recognize.h"

std::vector<int> a_xy, a_yx; // list of assignments that have been definitely confirmed;
std::vector<int> a2a; // which anchor (from str1) is bound to which anchor (in str2) - indices
std::vector<int> anchors1, anchors2;
std::vector<int> a_anch1, un_anch1;
extern bool debug; 

void Usage(std::string name){
  std::cerr << "Usage: " << name << "file1 file2 output" << std::endl;
  std::cerr << "  print skeletons" << std::endl;
}

class Metric{
public:
  Metric(){};
  Metric(std::vector<int> dist, std::vector<std::vector<bool> > cycle5, std::vector<std::vector<bool> > cycle6) : dist(dist), cycle5(cycle5), cycle6(cycle6){};
  std::vector<int> dist;
  std::vector<std::vector<bool> > cycle5;
  std::vector<std::vector<bool> > cycle6;
std::string type;
};

int overlap_measure(Metric metric1, Metric metric2){
  Metric un_copy = metric2;
if ( debug ){
 for ( auto it : metric1.dist ){ std::cout << it << " "; }; std::cout << std::endl;
 for ( auto it : un_copy.dist ){ std::cout << it << " "; }; std::cout << std::endl;
}
std::vector<bool> record;
bool check;

  std::vector<int>::iterator it2;
  int score = 0;
//  std::cout << "assigned: " << a_anch1.size() << std::endl;
//  std::cout << "unassigned: " << un_anch1.size() << std::endl;

//for ( auto it3 : a2a ){ std::cout << it3 <<  " "; }; std::cout << std::endl;

  for (int i = 0; i < anchors1.size(); i++ ){
    bool ordered = a2a[i] != -1;
    if ( ordered ){
//      std::cout << "ordered: " << i << " " << a2a[i] << std::endl;
      
        check = false;
        // check distances
        check = metric1.dist[i] == metric2.dist[a2a[i]];
        // check cycles
        if ( check ){
           
          check = check && ( metric1.cycle5[i].size() == 0 || ( metric1.cycle5[i] == metric2.cycle5[a2a[i]] && !std::all_of(metric1.cycle5[i].begin(), metric1.cycle5[i].end(), [](bool v) { return !v; }) ) || ( metric1.cycle6[i] == metric2.cycle6[a2a[i]] && !std::all_of(metric1.cycle6[i].begin(), metric1.cycle6[i].end(), [](bool v) { return !v; }) ) || ( std::all_of(metric1.cycle5[i].begin(), metric1.cycle5[i].end(), [](bool v) { return !v; }) && std::all_of(metric1.cycle6[i].begin(), metric1.cycle6[i].end(), [](bool v) { return !v; }) && std::all_of(metric2.cycle5[a2a[i]].begin(), metric2.cycle5[a2a[i]].end(), [](bool v) { return !v; }) && std::all_of(metric2.cycle6[a2a[i]].begin(), metric2.cycle6[a2a[i]].end(), [](bool v) { return !v; }) ) );
        }

//std::cout << i << " " << a2a[i] << std::endl;
//for ( auto it3 : metric1.cycle5[i] ){ std::cout << it3; }; std::cout << std::endl;
//for ( auto it3 : metric1.cycle6[i] ){ std::cout << it3; }; std::cout << std::endl;
//for ( auto it3 : metric2.cycle5[a2a[i]] ){ std::cout << it3; }; std::cout << std::endl;
//for ( auto it3 : metric2.cycle6[a2a[i]] ){ std::cout << it3; }; std::cout << std::endl;
//        for ( int j = 0; j < metric1.cycle5[i].size(); j++ ){
//          check = check & ( metric1.cycle5[i][j] == metric2.cycle5[a2a[i]][j] | metric1.cycle6[i][j] == metric2.cycle6[a2a[i]][j]);
//          std::cout << "b" << check;
//        }
//        std::cout << std::endl;
        if ( check ){ score++; }
    } else {
//      std::cout << "unordered: " << i << std::endl;
      if ( a2a[i] != -1 ){ continue; }
      check = false;
      int index;
      std::vector<int>::iterator it = std::find(un_copy.dist.begin(), un_copy.dist.end(), metric1.dist[i]);
      if ( it != un_copy.dist.end() ){ check = true; } // un_copy.dist.erase(it); }
//      std::cout << "looking for " << metric1.dist[i] << " " << bool(it == un_copy.dist.end()) << std::endl;
      while ( it != un_copy.dist.end() ){
        check = true;
        index = it - un_copy.dist.begin();
//          check = check & ( metric1.cycle5[i] == un_copy.cycle5[index] | metric1.cycle6[i] == un_copy.cycle6[index]);
          check = check && ( metric1.cycle5[i].size() == 0 || ( ( metric1.cycle5[i] == un_copy.cycle5[index] && !std::all_of(metric1.cycle5[i].begin(), metric1.cycle5[i].end(), [](bool v) { return !v; }) ) || ( metric1.cycle6[i] == un_copy.cycle6[index] && !std::all_of(metric1.cycle6[i].begin(), metric1.cycle6[i].end(), [](bool v) { return !v; }) ) ) || ( std::all_of(metric1.cycle5[i].begin(), metric1.cycle5[i].end(), [](bool v) { return !v; }) && std::all_of(metric1.cycle6[i].begin(), metric1.cycle6[i].end(), [](bool v) { return !v; }) && std::all_of(un_copy.cycle5[index].begin(), un_copy.cycle5[index].end(), [](bool v) { return !v; }) && std::all_of(un_copy.cycle6[index].begin(), un_copy.cycle6[index].end(), [](bool v) { return !v; }) ) );
//        for ( int j = 0; j < metric1.cycle5[i].size(); j++ ){
//          check = check & ( metric1.cycle5[i][j] == un_copy.cycle5[index][j] | metric1.cycle6[i][j] == un_copy.cycle6[index][j]);
//          std::cout << "b" << check;
//        }
/*        std::cout << std::endl;
*/
//std::cout << i << " " << index << " " << check << std::endl;
//for ( auto it2 : metric1.cycle5[i]){ std::cout << it2; }; std::cout << std::endl;
//for ( auto it2 : un_copy.cycle5[index]){ std::cout << it2; }; std::cout << std::endl;
//for ( auto it2 : metric1.cycle6[i]){ std::cout << it2; }; std::cout << std::endl;
//for ( auto it2 : un_copy.cycle6[index]){ std::cout << it2; }; std::cout << std::endl;
/*
*/

        it = std::find(it+1, un_copy.dist.end(), metric1.dist[i]);
        if ( check ){ //std::cout << i << " overlaps with " << index << std::endl;
          un_copy.dist.erase(std::next(un_copy.dist.begin(), index));
          un_copy.cycle5.erase(std::next(un_copy.cycle5.begin(), index));
          un_copy.cycle6.erase(std::next(un_copy.cycle6.begin(), index));
          break;
        }
      }
      if ( check ){ score++; }
    }
    record.push_back(check);
  }
  if ( debug ){
   std::cout << "record: ";
   for ( auto it : record ){ std::cout << it; }; std::cout << std::endl; 
   std::cout << "score: " << score << std::endl;
  }
  return 10 * score;
}

bool morph(Structure orig, Structure & target, std::vector<bool> isSkel){
//NEW:
  // 1. alignMaps - add skeleton check - if not identical, crash
  // 2. identify overlapping with uni_id - and steal XYZ
  // 3. Steal skeleton XYZ
  // 4. remaining - unidentified + non-overlapping with uni_id - identify which are contacts (bound to skeleton or overlapping w/uni_id) and create FG for each
  // 5. define anchors
  // 6. fill z-matrix
  // 7. reconstruct missing atoms
  // 8. orderUp FG
  // 9. reconstruct nonOverlapping


// Iterators:
// i - over indices
// nucl - over nucleotides
// u - over universal indices
// s - over skeletal indices

std::vector<FunctionalGroup> isolated;
Structure orig_str = target;

isolated.clear();

// 2. Steal xyz from Overlapping
for (int i = 1; i <=target.atoms.size(); i++){
//  std::cout << isSkel[i] << std::endl;
  if ( !isSkel[i] ){ continue; }
  target.atoms[i-1].setXYZ(orig.atoms[yx[i-1]].getXYZ());
//  std::cout << i << " is copied " << std::endl;
}

// 4. Find all reachable neighbours
bool is_contact;
int index; 

for ( int i = 1; i <=target.atoms.size(); i++ ){
  if ( isSkel[i] ){ continue; }
    is_contact = false;
    for ( std::set<Atom*>::iterator it = target.atoms[i-1].partners.begin(); it != target.atoms[i-1].partners.end(); it++ ){ // Check if j(uni) is touching skeleton => is a contact
      index = *it - &target.atoms[0] + 1; 
      if ( isSkel[index] ){ is_contact = true; break; }
      // the neigbour (index) is either a skeleton or art of overlap
    }
    if ( is_contact ) { // if this is a contact, create a functional group
//      isolated.push_back(FunctionalGroup(1,2,1));
      isolated.push_back(FunctionalGroup(i, index));
//      std::cout << i << " is a contact" << std::endl;
    }

}

// 5. Find anchor for each FunctionalGroup
  std::vector<int> path;
  int distance;

  for ( std::vector<FunctionalGroup>::iterator it = isolated.begin(); it != isolated.end(); it++ ){
    for ( int i = 1; i <= target.atoms.size(); i++){
      if ( ! isSkel[i] ){continue;}
      path.clear();
      distance = target.pathFinder(it->contact, i, path, 3);
      if ( distance == 3 ){
        it->anchor = path[2];
        it->way_to_anchor = path[1];
        it->skeleton_contact = path[0];
        break;
      }
    }
  }

// 6. Find which contact is the closest and assign us to this FG 
  int min_distance;
  std::vector<FunctionalGroup>::iterator home; // store the closest contact (i.e. what FG we belong to) - step 4
  std::vector<int> rem_path;

  for ( int i = 1; i <= target.atoms.size(); i++) {
    if ( isSkel[i] ){ continue; }

    min_distance = 999;
    for ( std::vector<FunctionalGroup>::iterator it = isolated.begin(); it != isolated.end(); it++ ){
      path.clear();
      distance = target.pathFinder(i, it->contact, path, -1);
      if ( distance > -1 && distance < min_distance ){ min_distance = distance; home = it; rem_path = path; } 
    }
    path = rem_path; // the shortest path
    if ( debug ){ std::cout << i << " is bound to " << home->contact << std::endl; }

//7. Fill FG
    home->indices.push_back(i);
    path.clear();
    target.pathFinder(i, home->anchor, path, -1);
    home->dist_to_anchor.push_back(path.size()); // necessary for reconstruction - build closer ones first
    home->ref1.push_back(path[0]);
    home->ref2.push_back(path[1]);
    home->ref3.push_back(path[2]);
//    for(int j = 1; j <= str.atoms.size(); j++){
    if ( home->contact == i ){
      for ( std::set<Atom*>::iterator it = target.atoms[home->skeleton_contact - 1].partners.begin(); it != target.atoms[home->skeleton_contact - 1].partners.end(); it++){
        index = *it - &target.atoms[0] + 1; 
        if ( debug ){ std::cout << "looking for newdih " << i << " " << isSkel[index] << " " << index << std::endl; }
        if ( isSkel[index] && index != home->way_to_anchor && index != i ){
          precision new_dih = orig.measure_dihedral(yx[index-1], yx[home->skeleton_contact-1], yx[home->way_to_anchor-1], yx[home->anchor-1]);
          home->correction.push_back(new_dih - orig_str.measure_dihedral(index - 1, home->skeleton_contact - 1, home->way_to_anchor - 1, home->anchor - 1) );
          if ( debug ){ std::cout << "new_dih " << i << " " << new_dih << " " << orig_str.measure_dihedral(index - 1, home->skeleton_contact - 1, home->way_to_anchor - 1, home->anchor - 1) << " " << home->correction.back() << " " << &*home << " " << home->correction.size() << std::endl; }
          break;
        }
      } 
    }
    if ( home->correction.size() == 0 ){ home->correction.push_back(0); }
// dihedrals do not work - geo is perturbed 
// check if ref1 is sp3 (120) or sp2 (180)
// if there is another atom bound to ref1 and not being ref2 that is in overlap - add the difference between str and rhs.str to dihedral


    home->distances.push_back( (orig_str.atoms[i - 1].getXYZ() - orig_str.atoms[path[0] - 1].getXYZ()).norm() ); //measure distance to ref1
    home->angles.push_back(orig_str.measure_angle(i - 1, path[0] - 1, path[1] - 1)); // measure angle ref2-ref1-me
    home->dihedrals.push_back(orig_str.measure_dihedral(i - 1, path[0] - 1, path[1] - 1, path[2] - 1) ); // measure dihedral ref3-ref2-ref1-me

  }
// 8. orderUp FG
//  for ( std::vector<FunctionalGroup>::iterator it = isolated.begin(); it != isolated.end(); it++ ){ it->orderUp(); }

std::set<int> all_to_rebuild;
for ( std::vector<FunctionalGroup>::iterator it = isolated.begin(); it != isolated.end(); it++ ){
  for ( auto it2 : it->indices ){ all_to_rebuild.insert(it2);}
}

// 9. reconstruct nonOverlapping
  Vector3p b1, b2, ref12, ref32; 
//if ( debug ){ std::cerr << isolated.size() << std::endl; }
  for ( std::vector<FunctionalGroup>::iterator it = isolated.begin(); it != isolated.end(); it++ ){
    it->orderUp();
    for ( int i = 0; i < it->indices.size(); i++ ){  
      ref12 = target.atoms[it->ref1[i] - 1].getXYZ() - target.atoms[it->ref2[i] - 1].getXYZ(); ref12.normalize();
      ref32 = target.atoms[it->ref3[i] - 1].getXYZ() - target.atoms[it->ref2[i] - 1].getXYZ(); ref32.normalize();
      b1 = ref32 - ref32.dot(ref12) * ref12; b1.normalize();
      b2 = ref12.cross(b1); b2.normalize();
     Vector3p probe;
     int neighbours = 0;
     bool conflict;
     precision dih_cor = it->dihedrals[i]; // dihedral incrementally modified until no clash is obtained; if you cant do it - dont modify
     precision conflict_val = 0;
     precision best_dih = dih_cor;
     if ( debug ){ std::cout << "Building " << it->indices[i] << std::endl; }
     do {
       probe = it->distances[i] * (cos(it->angles[i]*M_PI/180) * -1 * ref12 + sin(it->angles[i]*M_PI/180) * (cos((it->correction[i]+dih_cor)*M_PI/180) * b1 + sin((it->correction[i]+dih_cor)*M_PI/180) * b2) ) + target.atoms[it->ref1[i] - 1].getXYZ();
       conflict = false;
       for ( int j = 1; j <= target.atoms.size(); j++ ){
         if ( j != it->indices[i] && j != it->ref1[i] && all_to_rebuild.find(j) == all_to_rebuild.end() && ( target.atoms[j-1].getXYZ() - probe ).norm() < target.distThreshold(it->indices[i]-1, j-1) ){
           if ( ( target.atoms[j-1].getXYZ() - probe ).norm() > conflict_val ){ conflict_val = ( target.atoms[j-1].getXYZ() - probe ).norm(); best_dih = dih_cor; }
           conflict = true;
           break;

}
       }
       if ( ! conflict ){ best_dih = dih_cor; }
       dih_cor += 30;
     } while ( (dih_cor-it->dihedrals[i]-30) < 360 && conflict );

       probe = it->distances[i] * (cos(it->angles[i]*M_PI/180) * -1 * ref12 + sin(it->angles[i]*M_PI/180) * (cos((it->correction[i]+best_dih)*M_PI/180) * b1 + sin((it->correction[i]+best_dih)*M_PI/180) * b2) ) + target.atoms[it->ref1[i] - 1].getXYZ();


     target.atoms[it->indices[i] - 1].setXYZ(probe);
     all_to_rebuild.erase(it->indices[i]);

    }
  }
}

int main(int argc, char * argv[]){

Structure first, second, str1, str2;
std::vector<std::string> args(argv, argv + argc);
std::string file, file2;

file = args[1];
file2 = args[2];

bool assignment = false;
std::string output = "output.xyz";
bool cycle_req = false;

for ( std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++){
  if ( *it == "-d" ){ debug = true; }
  if ( *it == "-f" ){ file = *(it+1); file2 = *(it+2); }
  if ( *it == "-a" ){ assignment = true; }
  if ( *it == "-o" ){ output = *(it+1); }
  if ( *it == "-c" ){ cycle_req = true; }
}

if ( ! props::file_exists(file) || ! props::file_exists(file2) ) {
  Usage(argv[0]);
 return 1;
}

/*
first.readSTR(file);
second.readSTR(file2);

if ( second.atoms.size() < first.atoms.size() ){
  str1.readSTR(file2);
  str2.readSTR(file);
} else {
  str2.readSTR(file2);
  str1.readSTR(file);
}
*/

str1.readSTR(file);
str2.readSTR(file2);
str1.connectivityBasedOnDistance();
str2.connectivityBasedOnDistance();

// 

// make list of all atoms with at least 3 heavy neighbours or that are in a cycle
// these are (yet unassigned) atom indices called anchors - we will identify atoms based on relations to these

std::set<int> si_dummy;
std::vector<std::set<int> > setlist1, setlist2; 
std::vector<int> cycleindex1, cycleindex2;
std::vector<bool> cyclechange1, cyclechange2;

std::vector<bool> cycle51, cycle52, cycle61, cycle62; 

for ( int i = 1; i <= str1.atoms.size(); i++){
  cycle51.push_back(str1.InsideCycle(i, 5, si_dummy));
//  auto it = std::find(setlist1.begin(), setlist1.begin(), si_dummy);
//  if ( it == setlist1.end() ){ setlist1.push_back(si_dummy); }
//  it = std::find(setlist1.begin(), setlist1.begin(), si_dummy);
//  cycleindex51.push_back(it - setlist1.begin());
  cycle61.push_back(str1.InsideCycle(i, 6, si_dummy));
}
for ( int i = 1; i <= str2.atoms.size(); i++){
  cycle52.push_back(str2.InsideCycle(i, 5, si_dummy));
  cycle62.push_back(str2.InsideCycle(i, 6, si_dummy));
}


for ( int i = 0; i < str1.atoms.size(); i++){
//  if ( cycle51[i] ){ anchors1.push_back(i+1); continue; }
//  if ( cycle61[i] ){ anchors1.push_back(i+1); continue; }

  int heavy_neighbours = 0;
  if ( str1.atoms[i].period() > 1 ){ anchors1.push_back(i+1); }
//  for ( std::set<Atom*>::iterator it = str1.atoms[i].partners.begin(); it != str1.atoms[i].partners.end(); it++){
//    if ( (*it)->period() >= 2 ){ heavy_neighbours++; }
//  }
//  if ( heavy_neighbours >= 3 ){ anchors1.push_back(i+1); }
}

for ( int i = 0; i < str2.atoms.size(); i++){
//  if ( cycle52[i] ){ anchors2.push_back(i+1); continue; }
//  if ( cycle62[i] ){ anchors2.push_back(i+1); continue; }

  int heavy_neighbours = 0;
  if ( str2.atoms[i].period() > 1 ){ anchors2.push_back(i+1); }
//  for ( std::set<Atom*>::iterator it = str2.atoms[i].partners.begin(); it != str2.atoms[i].partners.end(); it++){
//    if ( (*it)->period() >= 2 ){ heavy_neighbours++; }
//  }
//  if ( heavy_neighbours >= 3 ){ anchors2.push_back(i+1); }
}

if ( debug ){
  std::cout << "anchors1" << std::endl;
  for ( auto it : anchors1 ){ std::cout << it << " "; }; std::cout << std::endl;
  std::cout << "anchors2" << std::endl; 
  for ( auto it : anchors2 ){ std::cout << it << " "; }; std::cout << std::endl;
}
// initialize
a_xy = std::vector<int>(N, -1);
if ( debug ){ for ( auto it : a_xy ){std::cout << it;}; }
a_yx = std::vector<int>(N, -1);
a2a  = std::vector<int>(anchors1.size(), -1);
for ( auto it : anchors1 ){ un_anch1.push_back(it); } 


std::vector<std::vector<int> > metric_dist1, metric_dist2;
std::vector< std::vector<std::vector<bool> > > metric_5cycle1, metric_5cycle2;
std::vector< std::vector<std::vector<bool> > > metric_6cycle1, metric_6cycle2;
std::vector<int> path;

for ( int i = 0; i < str1.atoms.size(); i++){
 if (debug){ std::cout << "constructing metric for str1 : " << i << std::endl; }
  std::vector<int> vi_dummy;
  std::vector<std::vector<bool> > vvb_dummy, vvb_dummy2;
  std::set<int> si_dummy;
  for ( std::vector<int>::iterator it2 = anchors1.begin(); it2 != anchors1.end(); it2++ ){
    std::vector<bool> vb_dummy;
    path.clear();
    str1.pathFinder(i+1, *it2, path, -1);
//    std::cout << "path " << i+1 << " to " << *it2 << std::endl;
//    for ( auto it : path){ std::cout << it << " ";}; std::cout << std::endl;
    vi_dummy.push_back(path.size());
    for ( auto it : path){ vb_dummy.push_back(cycle51[it-1]); }
    vvb_dummy.push_back(vb_dummy); vb_dummy.clear();
    for ( auto it : path){ vb_dummy.push_back(cycle61[it-1]); }
    vvb_dummy2.push_back(vb_dummy);
  }
if ( debug ){ for ( auto it : vi_dummy ){ std::cout << it << " "; }; std::cout << std::endl; }
  metric_dist1.push_back(vi_dummy);
  metric_5cycle1.push_back(vvb_dummy);
  metric_6cycle1.push_back(vvb_dummy2);
  //add 5-cycle and 6-cycle check
}
for ( int i = 0; i < str2.atoms.size(); i++){
 if ( debug ){ std::cout << "constructing metric for str2 : " << i << std::endl; }
  std::vector<int> vi_dummy;
  std::vector<std::vector<bool> > vvb_dummy, vvb_dummy2;
  std::set<int> si_dummy;
  for ( std::vector<int>::iterator it2 = anchors2.begin(); it2 != anchors2.end(); it2++ ){
    std::vector<bool> vb_dummy;
    path.clear();
    str2.pathFinder(i+1, *it2, path, -1);
    vi_dummy.push_back(path.size());
    for ( auto it : path){ vb_dummy.push_back(cycle52[it-1]); }
    vvb_dummy.push_back(vb_dummy); vb_dummy.clear();
    for ( auto it : path){ vb_dummy.push_back(cycle62[it-1]); }
    vvb_dummy2.push_back(vb_dummy);
  }
  if ( debug ){ for ( auto it : vi_dummy ){ std::cout << it << " "; }; std::cout << std::endl; }
  metric_dist2.push_back(vi_dummy);
  metric_5cycle2.push_back(vvb_dummy);
  metric_6cycle2.push_back(vvb_dummy2);
}

/*
while( metric_dist2.size() < metric_dist1.size() ){
  metric_dist2.push_back(std::vector<int>(anshors2.size(), INF));
  metric_5cycle2.push_back(vvb_dummy);
  metric_6cycle2.push_back(vvb_dummy2);
  
}
*/

if ( debug ){
  std::cout << "metrics constructed" << std::endl; 
  for ( int i = 0; i < str1.atoms.size(); i++){
    for ( auto it : metric_dist1[i] ){std::cout << it << " ";}; std::cout << std::endl;
  }
}
//

// complete cost matrix to square

if ( str1.atoms.size() < str2.atoms.size() ){
  for ( int i = str1.atoms.size(); i < str2.atoms.size(); i++){
    for ( int j = 0; j < str2.atoms.size(); j++){
      cost[i][j] = -1 * INF;
    }
  }
}
if ( str1.atoms.size() > str2.atoms.size() ){
  for ( int j = str2.atoms.size(); j < str1.atoms.size(); j++){
    for ( int i = 0; i < str1.atoms.size(); i++){
      cost[i][j] = -1 * INF;
    }
  }
}
//

while (un_anch1.size() > 0){ // start iterations
if ( debug ){ std::cout << "un_anch1 size = " << un_anch1.size() << std::endl; }

for (int i = 0; i < str1.atoms.size(); i++){
  for (int j = 0; j < str2.atoms.size(); j++){
    if ( a_xy[i] != -1 ){ cost[i][j] = -1 * INF; } // if i is assigned then exclude it from hungarian => -INF
    if ( a_yx[j] != -1 ){ cost[i][j] = -1 * INF; } // if j is assigned then exclude it from hungarian => -INF
    if ( a_xy[i] == j ){ cost[i][j] = INF; continue; } // if i is assigned to j then exclude it from hungarian => INF
//    std::cout << "constr cost " << i << " " << j << std::endl;
//    for ( auto it : metric_dist1[i] ){std::cout << it << " ";}; std::cout << std::endl;
    cost[i][j] = overlap_measure(Metric(metric_dist1[i], metric_5cycle1[i], metric_6cycle1[i]), Metric(metric_dist2[j], metric_5cycle2[j], metric_6cycle2[j])); // otherwise, use overlap_measure 
    if ( (str1.atoms[i].period() == 1) == (str2.atoms[j].period() == 1) ){ cost[i][j] += 5; }
    //if ( debug ){ std::cout << "cost matrix " << i << " " << j << " " << cost[i][j] << std::endl; }
  }
}

if ( debug ){
  std::cout << "cost matrix constructed" << std::endl;
  for ( int i = 0; i < str1.atoms.size(); i++){
    std::cout << "atom" << i+1 << " ";
    for ( int j = 0; j < str2.atoms.size(); j++){
      std::cout << cost[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

hungarian(N);
// pick the best element
int max = -1;
int buf,current;

//find unassigned anchor with best alignment
for ( int i = 0; i < un_anch1.size(); i++ ){ 
  buf = cost[un_anch1[i]-1][xy[un_anch1[i]-1]];
if ( debug ){  std::cout << i << " " << un_anch1[i] << " " << xy[un_anch1[i]-1]+1 << " " << cost[un_anch1[i]-1][xy[un_anch1[i]-1]] << std::endl; }
  if ( buf > max ){ max = buf; current = i; }
}
if ( max < std::min(anchors1.size(), anchors2.size()) * 5 ){ break; }
if ( debug ){ std::cout << "I have chosen to assign " << un_anch1[current] << " to " << xy[un_anch1[current]-1]+1 << " !" << current << std::endl; }
// add it to assigned anchors
a_anch1.push_back(un_anch1[current]);
// update a_xy and a_yx
a_xy[un_anch1[current]-1] = xy[un_anch1[current]-1];
a_yx[xy[un_anch1[current]-1]] = un_anch1[current]-1;
// update a2a
a2a[std::find(anchors1.begin(), anchors1.end(), un_anch1[current]) - anchors1.begin()] =  std::find(anchors2.begin(), anchors2.end(), a_xy[un_anch1[current]-1]+1) - anchors2.begin();
//std::cout << "a2a " << int(std::find(anchors1.begin(), anchors1.end(), un_anch1[current]) - anchors1.begin()) << " " << int(std::find(anchors2.begin(), anchors2.end(), a_xy[un_anch1[current]-1]+1) - anchors2.begin()) << std::endl;
//remove from unassigned
un_anch1.erase(std::next(un_anch1.begin(),current));
//

} // end of iterations



// Rebuild
std::vector<bool> isSkel(str2.atoms.size()+1, false); 

std::set<int> skel_set;

a_anch1 = un_anch1;
a2a  = std::vector<int>(anchors1.size(), -1);

for (int i = 0; i < str1.atoms.size(); i++){
  for (int j = 0; j < str2.atoms.size(); j++){
    cost[i][j] = overlap_measure(Metric(metric_dist1[i], metric_5cycle1[i], metric_6cycle1[i]), Metric(metric_dist2[j], metric_5cycle2[j], metric_6cycle2[j])); // otherwise, use overlap_measure 
    if ( (str1.atoms[i].period() == 1) == (str2.atoms[j].period() == 1) ){ cost[i][j] += 5; }
    if ( debug ){ std::cout << "cost matrix " << i << " " << j << " " << cost[i][j] << std::endl; }
  }
}

int mymax = 0;
for ( int i = 0; i < str1.atoms.size(); i++){
  for ( int j = 0; j < str2.atoms.size(); j++){
    if ( cost[i][j] > mymax ){ mymax = cost[i][j]; }
  }
}

for ( int i = 0; i < str1.atoms.size(); i++){
  if ( debug ){ std::cout << "atom" << i+1 << " "; }
  for ( int j = 0; j < str2.atoms.size(); j++){
//    if ( i == j ){ continue; }
//    path.clear();
//    str1.pathFinder(anchors1[i], anchors1[j], path, -1);
    if ( debug ){ std::cout << cost[i][j] << " "; }
//    if ( xy[i] == j && cost[i][j] == std::min(anchors1.size(), anchors2.size()) ){
    if ( xy[i] == j && cost[i][j] >= (mymax-6) ){

      isSkel[j+1] = true;
//      if ( debug ){ std::cout << i+1 << " " << j+1 << " = skel" << std::endl; }
    }
//    if ( debug ){ for ( auto it : path){ std::cout << it << " "; skel_set.insert(it); };std::cout << std::endl; }
  }
  if ( debug ){ std::cout << std::endl; }
}

if ( debug ){
  for ( int i = 1; i < isSkel.size(); i++){
    std::cout << i << " is skel " << isSkel[i] << std::endl;
  }
}

//if ( debug ){
//  std::cout << "skel" << std::endl;
//  for ( auto it : skel_set){ std::cout << it << " "; };std::cout << std::endl;
//}

//for ( auto it : skel_set ){
// isSkel[xy[it-1]+1] = true;
// if ( debug ){ std::cout << it << " is a skel " << std::endl; }
//}

if ( debug || assignment ){
  for ( int i = 0; i < str1.atoms.size(); i++){
    std::cout << i+1 << " " << xy[i]+1 << std::endl; 
  }
  return 0;
}

/*
if ( second.atoms.size() < first.atoms.size() ){
  for ( int i = 0; i < N; i++){
    yx[i] = xy[i];
  }
  str1.readSTR(file);
  str2.readSTR(file2);
  str1.connectivityBasedOnDistance();
  str2.connectivityBasedOnDistance();
}
*/

morph(str1, str2, isSkel);

str2.writeXYZ(output);

}

