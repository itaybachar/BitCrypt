#pragma once
#include <stdint.h>
#include <sys/types.h>

class FileEditor{
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
  FileEditor(const char* filepath, bool write = false);
  ~FileEditor();

  //Util
  FileEditor* prepareFile();
  void skip(uint32_t num);
  void deleteFile();

  //IO
  ssize_t readBytes(uint8_t* buf, uint32_t numBytes);
  ssize_t writeBytes(uint8_t* buf, uint32_t numBytes);
};
