#pragma once
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include<vector>
#include<fstream>
#include<eigen/Eigen/Core>
#include<eigen/Eigen/Geometry>
#include<string>
#include<set>

typedef float precision;
typedef Eigen::Matrix<precision, 3, 1> Vector3p;

class Structure;

class Atom{
public:
  Atom();
  Atom(Vector3p xyz, std::string type);
  Vector3p getXYZ() const;
  std::string getTYPE();
  void setXYZ(precision x, precision y, precision z);
  void setXYZ(Vector3p ref);
  int period() const;
  int valence() const;
  
  friend class Structure;
  friend class CDN;

//private:
  std::set<Atom*> partners;
  bool set; // true if xyz and type set properly
  std::string type;
  Vector3p xyz;
};

class Dihedral{
public:
  Atom * atoms[4];
  bool operator< (const Dihedral & rhs) const;
  bool operator> (const Dihedral & rhs) const;
  bool operator== (const Dihedral & rhs) const;
};

class Angle{
public:
  Atom * atoms[4];
  bool operator< (const Angle & rhs) const;
  bool operator> (const Angle & rhs) const;
  bool operator== (const Angle & rhs) const;
};

class Structure{
public:
  Structure();
  Structure(std::string file);
  Structure(const Structure & rhs);
  bool readSTR(std::string file);
  bool readXYZ(std::string file);
  bool readTMOL(std::string file);
  bool readSDF(std::string file);
  bool readCOSMO(std::string file);
  bool writeXYZ(std::string file, bool append = false);
  bool writeTMOL(std::string file, bool append = false);
  precision distThreshold(int i, int j);
  void connectivityBasedOnDistance();
  void dropConflictsBasedOnDistance();

  precision ghash();
  bool addPartner(int first, int second);
  bool remPartner(int first, int second);

  bool identicalConnectivity(const Structure & rhs);
  
  int distanceOverBonds(int first, int second, int max_depth = 6); // Breadth-first search, requires connectivity;
  int pathFinder(int first, int second, std::vector<int> & path, int max_depth = 6, bool cyclic = false, Atom * parent = NULL, int distance = 0);
  // find the shortest path from first to second and store it in path. Cyclic allows ignoring first == second. To keep track of where we are coming from we remember our parent. The function is recursive

  bool InsideCycle(int atom_id, int cycle_size, std::set<int> & indices);

  precision measure_dihedral(int a1, int a2, int a3, int a4) const;
  precision measure_dihedral(const Atom* a1, const Atom* a2, const Atom* a3, const Atom* a4) const; 
  precision measure_angle(int a1, int a2, int a3) const;
  precision measure_angle(const Atom* a1, const Atom* a2, const Atom* a3) const;
  void getDihedral(int depth, Dihedral & current, std::set<Dihedral> & my_dihedrals, bool unique = true);
  void getAngle(int depth, Angle & current, std::set<Angle> & my_angles);

  bool connected; // indicates whether connectivity has been read (either via reading SDF or connectivityBasedOnDistance
  std::vector<Atom> atoms; 
};


