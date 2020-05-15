#pragma once

class FileEditor{

private:
  uint32_t fd;
  uint32_t size;
public:
  FileEditor() {}
  
  bool loadFile();
  uint32_t read(uint8_t* buf, uint32_t numBytes)
  //write 
};
