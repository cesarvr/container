#ifndef CVZ_CGROUP
#define CVZ_CGROUP

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>


using namespace std;

class CGroupFS {
  private: 
    std::string folderName;
    std::ofstream ofs;

  public: 
    CGroupFS(std::string folder): folderName{folder} {
      mkdir( folder.c_str() , S_IRUSR | S_IWUSR);  // Read & Write
    }

    template <typename T>
    void write(std::string file, T value) {
      ofs.open(folderName + file, std::ofstream::out | std::ofstream::app);
      ofs << value; 
      ofs.close();
    }
};

void limitProcessCreationV1() { 
  mkdir( CGROUP_FOLDER, S_IRUSR | S_IWUSR);  // Read & Write

  CGroupFS cgroup{CGROUP_FOLDER}; 

  cgroup.write("pids.max", 5);
  cgroup.write("notify_on_release", 1);
  cgroup.write("cgroup.procs", getpid());
}



#endif
