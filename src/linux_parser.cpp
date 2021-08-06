#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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

float LinuxParser::MemoryUtilization() 
{
  string key,value;
  float usedMem ;
  string line;
  float fMemTotal=0;
  float fMemFree=0;
  std::ifstream fileStream(kProcDirectory+kMeminfoFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          fMemFree = std::stof(value);
        }
        if (key == "MemFree:") {
          fMemFree = std::stof(value);
        } 
      }
    usedMem = (fMemTotal-fMemFree)/fMemTotal;
    }
  }


  return usedMem;
}


long int LinuxParser::UpTime()
 { 
   string line;
   string sUpTime, sIdleTime;
   long upTime; ;
   std::ifstream fileStream(kProcDirectory+kUptimeFilename);
   if(fileStream.is_open())
   {
     std::getline(fileStream,line);
     std::istringstream lineStream(line);
    lineStream>>sUpTime>>sIdleTime;
   }

   upTime = std::stol(sUpTime);
   return upTime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

std::map<std::string,long> LinuxParser::CpuUtilization() 
{ 
    std::string cpu,user,nice,system ,idle,iowait,irq,softirq,steal,guest,guest_nice;
    std::map<string,long> processorMap;
    std::string line,key,value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open())
    {
        std::getline(filestream,line);
        std::istringstream linestream(line);
       linestream>>cpu>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;
        processorMap["user"] = std::stol(user);
        processorMap["nice"] = std::stol(nice);
        processorMap["system"] = std::stol(system);
        processorMap["idle"] = std::stol(idle);
        processorMap["iowait"] = std::stol(iowait);
        processorMap["irq"] = std::stol(irq);
        processorMap["softirq"] = std::stol(softirq);
        processorMap["steal"] = std::stol(steal);
        processorMap["guest"] = std::stol(guest);
        processorMap["guest_nice"] = std::stol(guest_nice);
        return processorMap;
    }  return {};

 }

int LinuxParser::TotalProcesses() 
{
  string line,key ,value;
  int processess;
  std::ifstream fileStream(kProcDirectory+kStatFilename);
  if(fileStream.is_open())
  {
    while(std::getline(fileStream,line))
    {
      std::istringstream lineStream(line);
      lineStream>>key>>value;
      if(key=="processes")
      {
        processess = std::stoi(value);
        return processess;
      }
    }
  }
   return 0;
}

int LinuxParser::RunningProcesses()
{
 string line,key,value;
 int runProcs;
 std::ifstream fileStream(kProcDirectory+kStatFilename);
 if(fileStream.is_open())
 {
   while(std::getline(fileStream,line))
   {
     std::istringstream lineStream(line);
     lineStream>>key>>value;
     if(key=="procs_running")
     {
       runProcs=std::stoi(value);
       return runProcs;
     }
   }
 }
 return 0;
}

string LinuxParser::Command(int pid[[]])
{ 
  string line;
  std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kCmdlineFilename); 
  if(fileStream.is_open())
  {
    std::getline(fileStream,line);
    return line;
  }
   return string(); 
}

string LinuxParser::Ram(int pid[[]]) 
{ 
  string line,key,value;
  float memUtil;
  std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kStatusFilename); 
  if(fileStream.is_open())
  {
    while(std::getline(fileStream,line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          memUtil = std::stof(value)/1000;
          value = std::to_string(memUtil);
          return value;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

string LinuxParser::User(int pid[[]])
{
  string line,key,value,uId;
  std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kStatusFilename); 
  if(fileStream.is_open())
  {
    while(std::getline(fileStream,line))
    {
    std::istringstream lineStream(line);
    lineStream>>key>>value;

    if(key == "Uid:")
    {
      uId = value;
    }
    }
  }
  fileStream.close();

  std::string name, password, id;
  fileStream.open(kPasswordPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> password >> id;
      if (id == uId) {
        return name;
      }
    }
  }
   return string(); 
}


long LinuxParser::UpTime(int pid[[]]) 
{ 
    long lSeconds;
    std::string line,value;
    int i;

    std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
    if(fileStream.is_open())
    {
      std::getline(fileStream, line);
      {
        std::istringstream lineStream(line);
        for(i=1;i<=22;i++)
        {
          lineStream>>value;
          if(i==22) {lSeconds = std::stol(value)/sysconf(_SC_CLK_TCK);}
        }
      }
    }
  return lSeconds;
}


std::map<std::string,long> LinuxParser::CpuUtilization(int pid[[]]) 
{ 
    std::map<string,long> processorMap;
    std::string line,key,value;
    long int Hertz;
    int i;

    std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
    if(fileStream.is_open())
    {
      std::getline(fileStream, line);
      {
        std::istringstream lineStream(line);
        for(i=1;i<=22;i++)
        {
          lineStream>>value;
          if(i==14) {processorMap["utime"] = std::stoi(value);}
          if(i==15) {processorMap["stime"] = std::stoi(value);}
          if(i==16) {processorMap["cutime"] = std::stoi(value);}
          if(i==17) {processorMap["cstime"] = std::stoi(value);}
          if(i==22) {processorMap["starttime"] = std::stoi(value);}
        }
      }
      Hertz= sysconf(_SC_CLK_TCK);
      processorMap["sHertz"] = Hertz;
    }

  return processorMap;

 }