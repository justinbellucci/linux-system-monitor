#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Processor {
 public:
  float Utilization();  

 private:
  float prevCpuIdleTime_ = 0.0;
  float prevCpuTotalTime_ = 0.0;

  vector<long> ConvertStringToLong(vector<string> cpu_data);
};

#endif