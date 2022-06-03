#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;
using namespace LinuxParser;


System::System()
{
    Processor myProcessor;
    cpu_= myProcessor; 
}
Processor& System::Cpu() { return cpu_; } //Selmarag: accessor function

vector<Process>& System::Processes()
{
  vector<Process> allProcesses{};
  vector<int> processIds = LinuxParser::Pids();
  for (int i : processIds) {
    allProcesses.push_back(i);
  }
    std::sort(allProcesses.begin(), allProcesses.end(),[]( Process& pa,  Process& pb) { return (pb.CpuUtilization() < pa.CpuUtilization()); });
  processes_ = allProcesses;

  return processes_;
}

std::string System::Kernel() {  return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() {return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime();}