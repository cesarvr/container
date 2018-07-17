#include <iostream>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>

int TRY(int status, const char *msg) {
 if(status == -1) {  
    perror(msg); 
    exit(EXIT_FAILURE);
 }
 return status;
}

void write_rule(const char* path, const char* value) {
  int fp = open(path, O_WRONLY | O_APPEND );
  write(fp, value, strlen(value));
  close(fp);
} 

#define CGROUP_FOLDER "/sys/fs/cgroup/pids/container/" 
#define concat(a,b) (a"" b)
void limitProcessCreation() {
  mkdir( CGROUP_FOLDER, S_IRUSR | S_IWUSR);  // Read & Write
  const char* pid  = std::to_string(getpid()).c_str();

  write_rule(concat(CGROUP_FOLDER, "pids.max"), "5"); 
  write_rule(concat(CGROUP_FOLDER, "notify_on_release"), "1"); 
  write_rule(concat(CGROUP_FOLDER, "cgroup.procs"), pid);
}

char* stack_memory() {
  const int stackSize = 65536;
  auto *stack = new (std::nothrow) char[stackSize];

  if (stack == nullptr) { 
    printf("Cannot allocate memory \n");
    exit(EXIT_FAILURE);
  }  

  return stack+stackSize;  //move the pointer to the end of the array because the stack grows backward. 
}

void setHostName(std::string hostname) {
  sethostname(hostname.c_str(), hostname.size());
}

void setup_variables() {
  clearenv();
  setenv("TERM", "xterm-256color", 0);
  setenv("PATH", "/bin/:/sbin/:usr/bin:/usr/sbin", 0);
}

template <typename... P> 
int run(P... params) {
  char *args[] = {(char *)params..., (char *)0};
  
  execvp(args[0], args); 
  perror("execvp"); 
  return 0;
}

int run2(const char *name) {
  char *_args[] = {(char *)name, (char *)0 };
  execvp(name, _args);
}

void setup_root(const char* folder){
  chroot(folder);
  chdir("/");
}

template <typename Function>
void clone_process(Function&& function, int flags){
 auto pid = TRY( clone(function, stack_memory(), flags, 0), "clone" );

 wait(nullptr); 
} 

#define lambda(fn_body) [](void *args) ->int { fn_body; };
 
int jail(void *args) {

  limitProcessCreation();
  printf("child pid: %d\n", getpid());
  setHostName("my-container");
  setup_variables();

  setup_root("./root");
  mount("proc", "/proc", "proc", 0, 0); 
  
  auto runnable = lambda(run("/bin/sh"))

  clone_process(runnable, SIGCHLD);

  umount("/proc");
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {


  printf("parent pid: %d\n", getpid());
  clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD );

  return EXIT_SUCCESS;
}
