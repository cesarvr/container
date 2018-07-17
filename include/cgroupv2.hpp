#ifndef CVZ_CGROUP
#define CVZ_CGROUP

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

struct OpenFile {
private:
  ofstream ofs;
  string name;

  string cgroupFolder = "/sys/fs/cgroup/pids/ccv/"; 
  int permissionFlags = S_IRUSR | S_IWUSR;

  void configureControlGroup(){
    mkdir( cgroupFolder.c_str() , permissionFlags);
  }

public: 
  OpenFile(string file) {
    configureControlGroup();
    name = cgroupFolder + file;
    ofs.open(name, std::ofstream::out | std::ofstream::app);
  }

  template <typename T>
  void write(T content) {
    cout << "adding " << content << " to ->" << name << endl;
    ofs << content; 
  }

  ~OpenFile() {
    cout << "closing " << name << " stream" << endl;
    ofs.close();
    chmod(name.c_str(), permissionFlags);
  }
}; 





#endif
