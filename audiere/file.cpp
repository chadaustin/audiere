#include "file.hpp"


////////////////////////////////////////////////////////////////////////////////

ADRFileSystem::ADRFileSystem(
  void* opaque,
  ADR_FILE_OPEN open,
  ADR_FILE_CLOSE close,
  ADR_FILE_READ read,
  ADR_FILE_SEEK seek,
  ADR_FILE_TELL tell)
{
  m_opaque = opaque;
  m_open   = open;
  m_close  = close;
  m_read   = read;
  m_seek   = seek;
  m_tell   = tell;
}

////////////////////////////////////////////////////////////////////////////////

IFile*
ADRFileSystem::Open(const char* filename)
{
  ADR_FILE file = m_open(m_opaque, filename);
  return (file ? new ADRFile(this, file) : 0);
}

////////////////////////////////////////////////////////////////////////////////

ADRFile::ADRFile(ADRFileSystem* file_system, ADR_FILE file)
{
  m_file_system = file_system;
  m_file = file;
}

////////////////////////////////////////////////////////////////////////////////

ADRFile::~ADRFile()
{
  m_file_system->m_close(m_file);
}

////////////////////////////////////////////////////////////////////////////////

int
ADRFile::Read(void* bytes, int byte_count)
{
  return m_file_system->m_read(m_file, bytes, byte_count);
}

////////////////////////////////////////////////////////////////////////////////

bool
ADRFile::Seek(int offset, ADR_SEEK_TYPE type)
{
  return (m_file_system->m_seek(m_file, offset, type) == ADR_TRUE);
}

////////////////////////////////////////////////////////////////////////////////

int
ADRFile::Tell()
{
  return m_file_system->m_tell(m_file);
}

////////////////////////////////////////////////////////////////////////////////
