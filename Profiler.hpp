#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <string>
#include <sys/time.h>
#include <map>

#define PROFILER_CALL Profiler p45asd(__FILE__, __func__, __LINE__);
#define PROFILER_OUTPUT_FILE(x) ProfilerHandler::getInstance()->setOutputFile(x);

class ProfilerHandler
{
  typedef struct {
    unsigned long   u_sec;
    unsigned int    calls;
  } _intern;
public:
  static ProfilerHandler  *getInstance();
  virtual ~ProfilerHandler();
  void  addData(std::string const &file, std::string const &function, int line, unsigned long ms);
  void  setOutputFile(const char *file);

private:
  ProfilerHandler();
  std::map<std::string,_intern> m_times;
  std::string m_outputFile;
  static ProfilerHandler *m_instance;
};

class Profiler
{
public:
  Profiler(const char *file, const char *functionName, int fileLine);
  virtual ~Profiler();

private:
  std::string m_file;
  std::string m_functionName;
  int         m_fileLine;
  struct timeval  m_timer;
};

#endif // PROFILER_HPP
