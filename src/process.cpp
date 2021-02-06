#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid) : processID_(pid) {
    // other initializers
}

// Return this process's ID
int Process::Pid() { return processID_; }

// Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() { 
    vector<float> cpuProcData = LinuxParser::CpuUtilizationProc(Pid());
    // totalProcTime = utime + stime + cutime + cstime
    float totalProcTime = cpuProcData[0] + cpuProcData[1] + cpuProcData[2] + cpuProcData[3];
    // elapsedTime in seconds = uptime - starttime
    float elapsedTime = Process::UpTime() - ( cpuProcData[4] / sysconf(_SC_CLK_TCK));
    float cpuProcUsage = (totalProcTime / sysconf(_SC_CLK_TCK)) / elapsedTime;
    return cpuProcUsage; 
}

// Return the command that generated this process
string Process::Command() { 
    command_ = LinuxParser::Command(Pid());
    return command_; }

// Return this process's memory utilization
string Process::Ram() { 
    long val = std::stol(LinuxParser::Ram(Pid()));
    val /= 1000; // MB conversion
    ram_ = std::to_string(val);
    return ram_; }

// Return the user (name) that generated this process
string Process::User() { 
    user_ = LinuxParser::User(Pid());
    return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime_ = LinuxParser::UpTime() - LinuxParser::UpTime(Pid());
    return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects

// bool Process::operator<(Process const &a) const { 
//     return (CpuUtilization() > a.CpuUtilization()); 
//     }


 