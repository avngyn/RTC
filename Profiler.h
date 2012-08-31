#ifndef PROFILER_H
#define PROFILER_H


#include <ctime>
#include <map>


class Profiler {
   public:
      Profiler();
      ~Profiler();

      void Start(const char *name);
      void Stop(const char *name);

      void Print(const char *name);
      void PrintAll();
      void PrintTotal();
   private:
      std::map<const char *, clock_t> start_times;
      std::map<const char *, clock_t> stop_times;
      std::map<const char *, double> elapsed_times;
};


#endif // PROFILER_H
