/*
 * Internal platform-specific threads
 *
 * AI namespace = Audiere Internal
 *
 */


#ifndef THREADS_HPP
#define THREADS_HPP


#include "Debug.hpp"


typedef void (*AI_ThreadRoutine)(void* opaque);

struct AI_CriticalSectionStruct;
typedef AI_CriticalSectionStruct* AI_CriticalSection;


// threads
bool AI_CreateThread(AI_ThreadRoutine routine, void* opaque, int priority = 0);

// waiting
void AI_Sleep(unsigned milliseconds);

// critical section
AI_CriticalSection AI_CreateCriticalSection();
void AI_DestroyCriticalSection(AI_CriticalSection cs);
void AI_EnterCriticalSection(AI_CriticalSection cs);
void AI_LeaveCriticalSection(AI_CriticalSection cs);


class Synchronized {
public:
  Synchronized() {
    m_cs = AI_CreateCriticalSection();
    // if it fails...  shit
  }

  ~Synchronized() {
    AI_DestroyCriticalSection(m_cs);
  }

  void Lock() {
    std::string s(name());
    ADR_LOG(("--> Trying to lock   " + s).c_str());
    AI_EnterCriticalSection(m_cs);
    ADR_LOG(("-->:: LOCKED ::   " + s).c_str());
  }

  void Unlock() {
    std::string s(name());
    ADR_LOG(("<-- Unlocking...   " + s).c_str());
    AI_LeaveCriticalSection(m_cs);
  }

  virtual const char* name() = 0;  // for debugging

private:
  AI_CriticalSection m_cs;
};


class AI_Lock {
public:
  AI_Lock(Synchronized* object)
  : m_object(object) {
    object->Lock();
  }

  ~AI_Lock() {
    m_object->Unlock();
  }

private:
  Synchronized* m_object;
};


#endif
