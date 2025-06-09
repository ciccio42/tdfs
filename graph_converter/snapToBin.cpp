#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
using namespace std;

#include <omp.h>

#include "def.h"

inline bool nextSNAPline(ifstream &infile, string &line, istringstream &iss, vidType &src, vidType &dest) {
	do {
    // std::cout << "\rreading line " << line << "\n";
    if(!getline(infile, line))
			return false;
	} while(line.length() == 0 || line[0] == '#' || line[0] == 'v'); 
	
  if (line[0] == 'e') {
    line = line.substr(2); // remove 'e '
    // remove the edge label if it exists
    // std::cout << "\rreading line " << line << std::flush;
  }
  iss.clear();
  iss.str(line);
	return !!(iss >> src >> dest);

}

inline void getID(vector<vidType> &idMap, vidType &id, vidType &nextID, bool vf_db) {
  if(idMap.size() <= id) {
    idMap.resize(id + 2048, (vidType)(-1));
  }
  if(idMap.at(id) == (vidType)(-1)) {
    if (vf_db){
      idMap.at(id) = id; // in vf_db, we do not remap ids
    }
    else{
      idMap.at(id) = nextID;
      nextID++;
    }
  }
  id = idMap.at(id);
}

void snapToBin(string fname, bool vf_db) {
  constexpr int inc = 65536;
  ifstream infile(fname.c_str());
  ofstream outfile((fname + ".bin").c_str(), ios::binary);
  ofstream out_rev((fname + ".rev.bin").c_str(), ios::binary);
  ofstream deg_out((fname + ".raw.degree.bin").c_str(), ios::binary);
	if(!infile || !outfile || !deg_out) {
		cout << "File not available\n";
		throw 1;
	}
  std::vector<uint64_t> degrees(inc);
	vidType nextID = 0;
  vector<vidType> idMap;
  idMap.reserve(2048);
  vidType max_id = 0;
  string line;
	istringstream iss;
	vidType edge[2];
  size_t lineNum = 0;
	while(nextSNAPline(infile, line, iss, edge[0], edge[1])) {
    if(++lineNum % 1000000 == 0) LOG("%lu edges read", lineNum);
    // std::cout << "src " << edge[0] << " dest " << edge[1] << "\n";
		getID(idMap, edge[0], nextID, vf_db);
    getID(idMap, edge[1], nextID, vf_db);
    max_id = std::max(edge[0], max_id);
    max_id = std::max(edge[1], max_id);
    if(max_id >= degrees.size()) degrees.resize(max_id + inc);
    degrees.at(edge[0])++;
    degrees.at(edge[1])++;
    edge[0] = __builtin_bswap32(edge[0]); // conversion to big-endian
    edge[1] = __builtin_bswap32(edge[1]);
    outfile.write(reinterpret_cast<const char*>(edge), 2*sizeof(vidType));
    std::swap(edge[0], edge[1]);
    out_rev.write(reinterpret_cast<const char*>(edge), 2*sizeof(vidType));
	}
	infile.close();
  outfile.close();
  cout << "\nwriting degrees\n" << std::flush;
  deg_out.write(reinterpret_cast<const char*>(degrees.data()), (max_id + 1) * sizeof(uint64_t));
  deg_out.close();
}

int main(int argc, char** argv) {
  bool vf_db = false;
  if(argc > 3) {
    cerr << "usage: ./convert <snap file>\n";
    return 1;
  }
  vf_db = bool(atoi(argv[2]));
  string fname = argv[1];
  snapToBin(fname, vf_db);
  cout << "snapToBin done\n" << std::flush;
	return 0;
}

