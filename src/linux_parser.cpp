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

using namespace LinuxParser;

template <typename T>
T GetValbyKey(string  FileName ,string  KeyVal)
{
  string line;
  string key;
  T value;
  std::ifstream fileStream( FileName);
  if(fileStream.is_open())
  {
    while(getline(fileStream,line))
    {
      std::istringstream lineStream(line);
      while(lineStream>>key>>value)
      {
        if (key == KeyVal)
        {
          return value;
        }
      }
    }
    fileStream.close();
  }
  return value;
}


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream fileStream(kOSPath);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
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
    fileStream.close();
  }
  return value;
}

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
  fMemTotal = std::stof(GetValbyKey<string>(kProcDirectory + kMeminfoFilename, "MemTotal:")) ;
  fMemFree = std::stof(GetValbyKey<string>(kProcDirectory + kMeminfoFilename, "MemFree:")) ;

  usedMem = (fMemTotal-fMemFree)/fMemTotal;


  return usedMem;
}


long int LinuxParser::UpTime()
 { 
   string line;
   string sUpTime;
   long upTime; ;
   std::ifstream fileStream(kProcDirectory+kUptimeFilename);
   if(fileStream.is_open())
   {
     std::getline(fileStream,line);
     std::istringstream lineStream(line);
    lineStream>>sUpTime;
    fileStream.close();
   }

   upTime = std::stol(sUpTime);
   return upTime;
 }


std::map<std::string,long> LinuxParser::CpuUtilization() 
{ 
    std::string cpu,user,nice,system ,idle,iowait,irq,softirq,steal,guest,guest_nice;
    std::map<string,long> processorMap;
    std::string line,key,value;
    std::ifstream fileStream(kProcDirectory + kStatFilename);
    if(fileStream.is_open())
    {
        std::getline(fileStream,line);
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
        fileStream.close();
        return processorMap;
    }  return {};

 }

int LinuxParser::TotalProcesses() 
{
  string line,key ,value;
  string sProcessess;
  sProcessess = GetValbyKey<string>(kProcDirectory+kStatFilename ,"processes");
  return sProcessess.empty()? 0 : stoi(sProcessess);
  
}

int LinuxParser::RunningProcesses()
{
 string line,key,value;
 string sRunProcs;
 sRunProcs = GetValbyKey<string>(kProcDirectory+kStatFilename ,"procs_running");
 return sRunProcs.empty()? 0 : stoi(sRunProcs);

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
  fileStream.close();
   return string(); 
}

string LinuxParser::Ram(int pid) 
{ 
  string sMemUtil;
  sMemUtil = GetValbyKey<string>(kProcDirectory+std::to_string(pid)+kStatusFilename ,"VmSize:");
  return sMemUtil.empty()? 0 : sMemUtil;
}

string LinuxParser::Uid(int pid)
{
  //return string();
  return GetValbyKey<string>(kProcDirectory + to_string(pid) + kStatusFilename,"Uid:");
}



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
  fileStream.close();
   return string(); 
}


long LinuxParser::UpTime(int pid[[]]) 
{ 
  std::string line;
	string value;
	vector<string> values;

    std::ifstream fileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
    if(fileStream.is_open())
    {
      std::getline(fileStream, line);
      {
        std::istringstream lineStream(line);
        while (lineStream >> value)
        {
          values.push_back(value);
        }
      }
	  fileStream.close();
	}
	int upTimePid = UpTime() -stol(values[21])/sysconf(_SC_CLK_TCK);
	return upTimePid;
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
  fileStream.close();
  return processorMap;

 }