#include "processor.h"
#include "linux_parser.h"


using namespace LinuxParser;
using std::string;


Processor::Processor():prevIdle(0),prevNonIdle(0),prevTotal(0){};
float Processor::Utilization()
 {

    std::map<std::string,long> processorMap =  LinuxParser::CpuUtilization();
     float CPU_Percentage;
     long idle,nonIdle,total,totald,idled;
     idle = processorMap["idle"]+  processorMap["iowait"];
     nonIdle =  processorMap["user"]+ processorMap["nice"]+processorMap["system"]+processorMap["irq"]+processorMap["softirq"]+processorMap["steal"];
     


     total = processorMap["idle"] + nonIdle;
     totald = total - prevTotal;
     idled = idle - prevIdle;


     prevIdle = idle;
     prevNonIdle = nonIdle;
     prevTotal = total;

     CPU_Percentage = (totald - idled)/totald;

     return CPU_Percentage;
 }
