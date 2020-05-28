#pragma once
#include <stdint.h>
#include <sys/types.h>

class FileEditor{
private:

  uint8_t* prebuf;
  uint8_t* buf;
  uint8_t* cur;
  ssize_t avail;

  int32_t fd; //File Descriptor 
  uint32_t size; //Size of current file
  uint32_t loc;
  uint32_t flags;
  bool isWrite;

  const char* filepath;
public:
  static const uint32_t SIZE = 32768;
  const uint32_t preBufSize = 64;
private:
  //Read the next chunk of memory if possible
  void readChunk();

public:
  FileEditor();
  ~FileEditor();

  //IO
  bool loadFile(const char filepath[], bool write = false);
  int32_t readBytes(uint8_t* buf, uint32_t numBytes);
  int32_t writeBytes(uint8_t* buf, uint32_t numBytes);
  bool closeFile();

  //Helper
  bool wipeFile();
  uint32_t fileSize();
  uint32_t filePointerLoc();
  uint8_t* getBuffer();
  void makeTemp();
};
