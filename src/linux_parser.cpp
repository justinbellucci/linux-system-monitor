#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include "linux_parser.h"
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string num;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> num;
  }
  return kernel + " " + num;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTot; 
  float memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "MemTotal") {
          memTot = std::stof(value);
        }
        else if(key == "MemFree") {
          memFree = std::stof(value);
          break;
        }
      }
    }
  }   
  filestream.close();
  return (memTot - memFree) / memTot;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string totaltime;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> totaltime;
    } 
  return std::stof(totaltime); 
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_data;
  string line;
  string key;
  string duser, dnice, dsystem, didle, diowait, dirq, dsoftirq, dsteal, dguest, dguest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> duser >> dnice >> dsystem >> didle >> diowait >> dirq >> dsoftirq >> dsteal >> dguest >> dguest_nice){
        if(key == "cpu"){
          cpu_data.push_back(duser);
          cpu_data.push_back(dnice);
          cpu_data.push_back(dsystem);
          cpu_data.push_back(didle);
          cpu_data.push_back(diowait);
          cpu_data.push_back(dirq);
          cpu_data.push_back(dsoftirq);
          cpu_data.push_back(dsteal);
          cpu_data.push_back(dguest);
          cpu_data.push_back(dguest_nice);
          return cpu_data;
        }
      }
    }
  }
  return cpu_data; 
  }

// Read and return the process CPU Status
std::vector<float> LinuxParser::CpuUtilizationProc(int pid) {
std::vector<float> cpuProcData;
string line;
string value;
float procTime = 0.0;
std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
if(filestream.is_open()){
  while(std::getline(filestream, line)){
    std::istringstream linestream(line);
    for(int i=1; i<=kstarttime_; i++){
      linestream >> value;
      // look for item num 14, 15, 16, 17 in stat file
      if(i==kutime_ || i==kstime_ || i==kcutime_ || i==kcstime_ || i==kstarttime_){
        procTime = std::stof(value) / sysconf(_SC_CLK_TCK);
        cpuProcData.push_back(procTime);
      }
    }
  }
}
return cpuProcData;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key; 
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "processes"){
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int value;
  //int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "procs_running"){
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
  }
  return line; 
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "VmData"){
          return value;
        }
      }
    }
  }
  return value; 
  }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value; 
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
      if(key == "Uid"){
        return value;
      }
     }
    }
  }
  return value; 
  }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  string key;
  string name;
  string x;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> name >> x >> key){
        if(key == uid){
          return name;
        }
      }
    }
  }
  return name; 
  }

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  int loc = 1;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value && loc < kstarttime_){
      ++loc;
    }
  } 
  return std::stol(value) / sysconf(_SC_CLK_TCK);
}
