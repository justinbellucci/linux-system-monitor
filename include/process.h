#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
    Attributes:
        Pid: (int)
        User: (string)
        Command: (string)
        CpuUtilization: (float)   
        Ram: (string)
        UpTime: (long)  
*/
class Process {
 public:
  Process(int pid); // Constructor to initialize process with PID

  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const &a) const;  // TODO: See src/process.cpp

 private:
    int processID_;
    std::string user_;
    std::string command_;
    std::string ram_;
    long uptime_;
};

#endif