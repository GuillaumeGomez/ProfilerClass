#include "Profiler.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

ProfilerHandler *ProfilerHandler::m_instance = 0;

static void freeProfilerHandler()
{
  ProfilerHandler *t = ProfilerHandler::getInstance();

  delete t;
}

ProfilerHandler *ProfilerHandler::getInstance()
{
  if (!m_instance) {
      m_instance = new ProfilerHandler;
      atexit(freeProfilerHandler);
    }
  return m_instance;
}

ProfilerHandler::ProfilerHandler()
{
}

ProfilerHandler::~ProfilerHandler()
{
  if (m_outputFile.empty())
    for (std::map<std::string, _intern>::iterator it = m_times.begin(); it != m_times.end(); ++it) {
        std::cout << it->first << " -> " << it->second.u_sec << " microsecond(s) and called: " << it->second.calls << " time(s)" << std::endl;
      }
  else {
      std::ofstream myfile;
      myfile.open(m_outputFile.c_str());
      if (myfile.is_open())
        for (std::map<std::string, _intern>::iterator it = m_times.begin(); it != m_times.end(); ++it) {
            myfile << it->first << " -> " << it->second.u_sec << " microsecond(s) and called: " << it->second.calls << " time(s)" << std::endl;
          }
      else
        std::cerr << m_outputFile << ": error with file" << std::endl;
      myfile.close();
    }
}

void  ProfilerHandler::addData(std::string const &file, std::string const &function, int line, unsigned long ms)
{
  std::stringstream s;
  s << line;
  std::string key = file + "/" + function + ":" + s.str();
  std::map<std::string, _intern>::iterator it = m_times.find(key);

  if (m_times.end() != it) {
      it->second.u_sec += ms;
      it->second.calls += 1;
    } else {
      _intern t;

      t.u_sec = ms;
      t.calls = 1;
      m_times[key] = t;
    }
}

void  ProfilerHandler::setOutputFile(const char *file)
{
  if (file)
    m_outputFile = file;
}

Profiler::Profiler(const char *file, const char *functionName, int fileLine)
  : m_file(file), m_functionName(functionName), m_fileLine(fileLine)
{
  gettimeofday(&m_timer, NULL);
}

Profiler::~Profiler()
{
  struct timeval tmp;
  unsigned long diff;

  gettimeofday(&tmp, NULL);
  diff = (tmp.tv_sec - m_timer.tv_sec) * 1000000 + (tmp.tv_usec - m_timer.tv_usec);
  ProfilerHandler::getInstance()->addData(m_file, m_functionName, m_fileLine, diff);
}
