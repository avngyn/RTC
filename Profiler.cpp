#include <iomanip>
#include <iostream>

#include "Profiler.h"


Profiler::Profiler() {
   start_times.clear();
   elapsed_times.clear();
}

Profiler::~Profiler() {
}

void Profiler::Start(const char *name) {
   start_times[name] = std::clock();
}

void Profiler::Stop(const char *name) {
   stop_times[name] = std::clock();
   elapsed_times[name] = (stop_times[name] - start_times[name]);
   this->Print(name);
}

void Profiler::Print(const char *name) {
   std::cout << std::left << std::setw(25) << name << std::setw(15) << start_times[name]/(double)CLOCKS_PER_SEC << std::setw(15) << stop_times[name]/(double)CLOCKS_PER_SEC << std::setw(15) << elapsed_times[name]/(double)CLOCKS_PER_SEC << std::endl;
}

void Profiler::PrintAll() {
   std::map<const char *, double>::reverse_iterator it;
   const char *name;
   std::cout << std::left << std::setw(25) << "Name" << std::setw(15) << "Start" << std::setw(15) << "Stop" << std::setw(15) << "Elapsed" << std::endl;
   for (it = elapsed_times.rbegin(); it != elapsed_times.rend(); it++) {
      name = it->first;
      std::cout << std::left << std::setw(25) << name << std::setw(15) << start_times[name]/(double)CLOCKS_PER_SEC << std::setw(15) << stop_times[name]/(double)CLOCKS_PER_SEC << std::setw(15) << elapsed_times[name]/(double)CLOCKS_PER_SEC << std::endl;
   }
}

void Profiler::PrintTotal() {
   std::map<const char *, double>::reverse_iterator it;
   double total;
   int minutes, seconds;
   total = 0;
   for (it = elapsed_times.rbegin(); it != elapsed_times.rend(); it++) {
      total += it->second;
   }
   minutes = (int)total/CLOCKS_PER_SEC/60;
   seconds = ((int)total/CLOCKS_PER_SEC)%60;
   std::cout << std::left << std::setw(25) << "TOTAL ELAPSED:" << minutes << "m" << seconds << "s" << std::endl;
}
