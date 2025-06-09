#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <parallel/numeric>
#include <cstring>
#include <sstream>
#include "def.h"


template<typename T>
T* custom_alloc_local(size_t elements) {
  return new T[elements];
}
template<typename T>
T* custom_alloc_global(size_t elements) {
  return new T[elements];
}
template<typename T>
void custom_free(T *ptr, size_t elements) {
  delete[] ptr;
}
template<typename T>
static void read_file(std::string fname, T *& pointer, size_t elements) {
  pointer = custom_alloc_global<T>(elements);
  assert(pointer);
  std::ifstream inf(fname.c_str(), std::ios::binary);
  if(!inf.good()) {
    std::cerr << "Failed to open file: " << fname << "\n";
    exit(1);
  }
  inf.read(reinterpret_cast<char*>(pointer), sizeof(T) * elements);
  inf.close();
}

typedef uint32_t vidType; // type of vertex id fields, 32-bit works up to 4 bill
void convert(std::string prefix, bool vf_db){

  vidType n_vertices, *edges;
  uint64_t n_edges, *vertices;
  std::srand(0);
  std::ifstream f_meta((prefix + ".meta.txt").c_str());
  assert(f_meta);
  int vid_size;
  vidType max_degree;
  f_meta >> n_vertices >> n_edges >> vid_size >> max_degree;
  assert(sizeof(vidType) == vid_size);
  f_meta.close();
  read_file(prefix + ".vertex.bin", vertices, n_vertices+1);
  auto labels = MemoryMappedFile<uint32_t>::Write(prefix + ".label.bin", n_vertices);
  if (!vf_db){
    for(int i=0;i<n_vertices;++i){
      labels[i]=std::rand()%4;
    }
  }else{
    // open snap file
    std::ifstream snap_file(prefix.c_str());
    std::istringstream iss;
    // read lines
    std::string line;
    vidType id;
    vidType label;
    getline(snap_file, line); // skip first line
    while(line.length() > 0) 
    {
      if (line[0] == 'v')
      {
        // get vertex id and label
        // std::cout << "Reading line: " << line << "\n";
        iss.clear();
        line = line.substr(2);
        iss.str(line);
        iss >> id >> label;
        // std::cout << "Vertex ID: " << id << ", Label: " << label << "\n";
        labels[id] = label;
      }
      getline(snap_file, line);
    }
  }
}

int main(int argc, char **argv) {
  bool vf_db = false;
  if (argc > 3) {
    std::cerr << "usage: ./labelize <snap file>\n";
    return 1;
  }
  vf_db = bool(atoi(argv[2]));
  std::string fname = argv[1];
  convert(fname, vf_db);
  return 0;
}

