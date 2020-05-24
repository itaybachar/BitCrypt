#pragma once
#include <stdint.h>

class FileEditor{

private:
  int32_t fd; //File Descriptor 
  uint32_t size; //Size of current file
  uint32_t loc;
  uint32_t flags;
  uint32_t avail;
  uint8_t* buf;

  const char* filepath;
  bool truncate; //Delete contents on first write
private:
public:
  FileEditor();
  ~FileEditor();

  //IO
  bool loadFile(const char filepath[], bool truncate = true);
  int32_t readBytes(uint8_t* buf, uint32_t numBytes);
  int32_t writeBytes(uint8_t* buf, uint32_t numBytes);
  bool closeFile();

  //Helper
  bool wipeFile();
  uint32_t fileSize();
  uint32_t filePointerLoc();

  inline uint8_t* getBuffer() { return this->buf; }
};
