#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  

  
 private:
 long prevIdle,prevNonIdle,prevTotal;

};

#endif