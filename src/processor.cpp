#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    vector<long> cpu_data;
    cpu_data = ConvertStringToLong(LinuxParser::CpuUtilization());
    // userTime = User - Guest
    float userTime = cpu_data[LinuxParser::kUser_] - cpu_data[LinuxParser::kGuest_];
    // niceTime = Nice - GuestNice
    float niceTime = cpu_data[LinuxParser::kNice_] - cpu_data[LinuxParser::kGuestNice_];
    // cpuIdleTime = Idle + IOwait
    float cpuIdleTime = cpu_data[LinuxParser::kIdle_] + cpu_data[LinuxParser::kIOwait_];
    // systemTimeTot = System + IRQ + SoftIRQ
    float systemTimeTot = cpu_data[LinuxParser::kSystem_] + cpu_data[LinuxParser::kIRQ_] + cpu_data[LinuxParser::kSoftIRQ_];
    // virtualTime = Guest + GuestNice
    float virtualTime = cpu_data[LinuxParser::kGuest_] + cpu_data[LinuxParser::kGuestNice_];
    // nonIdleTime = user + nice + system + irq + softirq + steal
    float nonIdleTime = cpu_data[LinuxParser::kUser_] + cpu_data[LinuxParser::kNice_] + systemTimeTot + cpu_data[LinuxParser::kSteal_];

     // cpuTotalTime = userTime + niceTime + systemTimeTot + cpuIdleTime + steal + virtualTime
    float cpuTotalTime = userTime + niceTime + systemTimeTot + cpuIdleTime + 
                         cpu_data[LinuxParser::kSteal_] + virtualTime;

    // calculate cpu percentage
    float cpuIdleDiff = cpuIdleTime - prevCpuIdleTime_;        
    float cputTotalDiff = cpuIdleTime + nonIdleTime - prevCpuTotalTime_;
    // CPU_percentage
    float cpu_percentage = (cputTotalDiff - cpuIdleDiff) / cputTotalDiff;

    prevCpuIdleTime_ = cpuIdleTime;
    prevCpuTotalTime_ = cpuTotalTime;

    return cpu_percentage;
}

vector<long> Processor::ConvertStringToLong(vector<string> cpu_data) {
    vector<long> convertedValues;
    int vecsize = cpu_data.size();
    for(int i = 0; i < vecsize; i++){
        convertedValues.push_back(std::stol(cpu_data[i]));
    }
    return convertedValues;
}