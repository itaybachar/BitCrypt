#pragma once
#include <stdint.h>

class FileEditor{

private:
  int32_t fd; //File Descriptor 
  uint32_t size; //Size of current file
private:
  bool _closeFile();
public:
  FileEditor();
  ~FileEditor();

  //IO
  bool loadFile(const char filepath[]);
  uint32_t readBytes(uint8_t* buf, uint32_t numBytes);
  uint32_t writeBytes(uint8_t* buf, uint32_t numBytes);

  //Helper
  uint32_t fileSize();
};
