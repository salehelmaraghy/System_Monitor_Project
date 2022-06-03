#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int Pid) : pid_(Pid) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization () 
{ 
    std::map <std::string,long> cpuProcess;
    float total_time,seconds,cpu_usage;
    cpuProcess = LinuxParser::CpuUtilization(Pid());
    total_time = cpuProcess["utime"] + cpuProcess["stime"];
    total_time = total_time+ cpuProcess["cutime"] + cpuProcess["cstime"];
    seconds = LinuxParser::UpTime() - ( cpuProcess["starttime"] /  cpuProcess["sHertz"]);
    cpu_usage =  ((total_time / cpuProcess["sHertz"]) / seconds);

    return cpu_usage; 
 }

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid());}

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process & a)
{ 
    return (CpuUtilization()<a.CpuUtilization()) ; 
}

bool Process::operator>(Process & a) {
  return CpuUtilization() > a.CpuUtilization();
}
