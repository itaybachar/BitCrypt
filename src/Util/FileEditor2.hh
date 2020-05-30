#pragma once
#include <stdint.h>
#include <sys/types.h>

class FileEditor2{
private:
  uint8_t* m_prebuf;
  uint8_t* m_buf;
  uint8_t* m_cur;
  ssize_t m_avail;

  int32_t m_fd; //File Descriptor 
  uint32_t m_flags; //File Flags
  const char* m_filepath;
public:
  static const uint32_t s_SIZE = 32768;
  static const uint32_t s_PREBUFSIZE = 64;
private:
  //Read the next chunk of memory if possible
  void readChunk();
public:
  //Constructors
  FileEditor2(const char* filepath, bool write = false);
  ~FileEditor2();

  //Util
  FileEditor2* prepareFile();

  //IO
  ssize_t readBytes(uint8_t* buf, uint32_t numBytes);
  ssize_t writeBytes(uint8_t* buf, uint32_t numBytes);
};
