#include "FileEditor.hh"
#include <sys/stat.h>
#include <fcntl.h>


bool FileEditor::loadFile(const char* filepath){
  this->fd = open(filepath,O_RDWR);
  if(fd<0){
    puts("ERROR: Could not find file");
    return false;
  }

  struct stat buf;
  fstat(fd,&buf);
  this->size = buf.st_size;
}

//Return num bytes read
uint32_t FileEditor::read(uint8_t* buf,uint32_t numBytes){
  
}
